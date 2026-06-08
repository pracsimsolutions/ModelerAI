// LLM-assisted dedup pass over the ingested manual topics.
//
// STATUS: ready to run. Three modes:
//
//   --dry-run      Walk topics, no API calls. Useful for seeing which
//                  topics would be processed.
//   (default)      Classify pass — calls Anthropic for each topic in
//                  breadcrumb order, writes tools/dedup-manifest.json
//                  with KEEP/MERGE/DROP decisions. NON-destructive.
//   --apply        After a classify pass has written the manifest, this
//                  reads the manifest and rewrites topic files:
//                    DROP   → delete the .md
//                    MERGE  → replace body with a pointer stub:
//                              "See <canonical-id> for the canonical text."
//                              plus the original frontmatter so it stays
//                              findable by id.
//                    KEEP   → no change.
//                  Always re-run `node tools/build-kb.js` after --apply
//                  to refresh INDEX.json.
//
// Suggested workflow:
//   1. node tools/dedup-manual.js               (writes manifest; ~$2-5)
//   2. Review tools/dedup-manifest.json by hand. Spot-check 10-20 MERGE
//      decisions — that's where mistakes matter most. Fix individual
//      decisions to "keep" in the manifest if the model was wrong.
//   3. node tools/dedup-manual.js --apply       (destructive; rewrites files)
//   4. node tools/build-kb.js                   (refresh search index)
//   5. git diff to verify the changes look sane before committing.
//
// Why this exists:
//   ingest-manual.js produces 124 topics from FlexSim's manual. Many
//   topics restate the same concept ("how Sources work" appears in
//   Sources overview + Conveyors flow chapter + the BestPractices intro).
//   BM25 search still finds the right topic, but each retrieval pays
//   token cost for redundant text. A dedup pass:
//     - Drops near-duplicate explanations (keep the most canonical
//       version, cross-link the others).
//     - Strips orientation chrome ("In this section you'll learn…").
//     - Merges single-concept fragments scattered across chapters into
//       one canonical topic.
//
// Strategy: SEQUENTIAL review. Cheaper than pairwise compare (which
// would be 124² = 15K LLM calls). The pass reads every topic in
// breadcrumb-ordered groups and maintains a "canonical concepts" map.
// For each new topic, the LLM decides: KEEP, MERGE_INTO (point at an
// existing canonical), or DROP.
//
// Cost estimate (Claude Haiku 4.5, USD pricing as of 2026-06):
//   - 124 topics × ~3K input tokens each (full body for context) = 372K input tokens
//   - + canonical-map context grows each call: assume ~10K avg = 1.24M input tokens
//   - Output: ~500 tokens × 124 = 62K output tokens
//   - At $1/MTok input + $5/MTok output: ~$1.61 input + $0.31 output ≈ $2/run
//   With Sonnet 4.6 (~$3/MTok input + $15/MTok output): ~$3.72 input + $0.93 output ≈ $5/run
//
// First-run cost is negligible. Per-FlexSim-version re-runs are similar.
//
// Usage (once tuned):
//   set ANTHROPIC_API_KEY=...     (or export on POSIX)
//   node tools/dedup-manual.js [--model haiku|sonnet] [--dry-run]
//
// Output:
//   - KNOWLEDGE/topics/manual/<id>.md may be REWRITTEN (canonical) or
//     REPLACED with a stub (merged/dropped).
//   - tools/dedup-manifest.json records every decision (audit trail).
//
// Dependency-free (uses Node 18+ built-in fetch).
'use strict';

const fs   = require('fs');
const path = require('path');

const repoRoot   = path.join(__dirname, '..');
const manualDir  = path.join(repoRoot, 'KNOWLEDGE', 'topics', 'manual');
const manifestPath = path.join(__dirname, 'dedup-manifest.json');

// -----------------------------------------------------------------------
// Args.
// -----------------------------------------------------------------------
const args = process.argv.slice(2);
const dryRun = args.includes('--dry-run');
const applyMode = args.includes('--apply');
const modelArg = args.find(a => a.startsWith('--model='));
const model = modelArg ? modelArg.split('=')[1] :
  (args.includes('--model') ? args[args.indexOf('--model') + 1] : 'haiku');

const MODEL_ID = ({
  haiku:  'claude-haiku-4-5-20251001',
  sonnet: 'claude-sonnet-4-6',
}[model] || 'claude-haiku-4-5-20251001');

const apiKey = process.env.ANTHROPIC_API_KEY;
if (!apiKey && !dryRun && !applyMode) {
  console.error('ANTHROPIC_API_KEY not set. Use --dry-run to preview without API calls,');
  console.error('or --apply to rewrite topic files from an existing manifest (no API needed).');
  process.exit(1);
}

// -----------------------------------------------------------------------
// Load + group topics.
// -----------------------------------------------------------------------
function loadTopics() {
  if (!fs.existsSync(manualDir)) {
    console.error(`No manual topics found at ${manualDir}. Run ingest-manual.js first.`);
    process.exit(1);
  }
  const out = [];
  for (const fname of fs.readdirSync(manualDir)) {
    if (!fname.endsWith('.md')) continue;
    const full = path.join(manualDir, fname);
    const raw = fs.readFileSync(full, 'utf8');
    const fm = parseFrontmatter(raw);
    out.push({
      id: fm.id || fname.replace(/\.md$/, ''),
      name: fm.name || fname,
      breadcrumb: fm.breadcrumb || [],
      body: stripFrontmatter(raw),
      path: full,
    });
  }
  // Group by top-level chapter so the canonical-map context stays focused.
  // Topics within the same chapter are more likely to overlap than topics
  // across chapters.
  out.sort((a, b) => a.id.localeCompare(b.id));
  return out;
}

function parseFrontmatter(text) {
  if (!text.startsWith('---')) return {};
  const end = text.indexOf('\n---', 3);
  if (end < 0) return {};
  const fm = text.slice(3, end).replace(/^\n/, '');
  const out = {};
  for (const line of fm.split('\n')) {
    const m = line.match(/^([a-zA-Z_][\w-]*):\s*(.*)$/);
    if (!m) continue;
    const key = m[1];
    const v = m[2].trim();
    try { out[key] = JSON.parse(v); } catch { out[key] = v.replace(/^"|"$/g, ''); }
  }
  return out;
}
function stripFrontmatter(text) {
  if (!text.startsWith('---')) return text;
  const end = text.indexOf('\n---', 3);
  if (end < 0) return text;
  return text.slice(end + 4).replace(/^\n/, '');
}

// -----------------------------------------------------------------------
// LLM call.
// -----------------------------------------------------------------------
async function classifyTopic(topic, canonicalSummaries) {
  // TODO(tune): the dedup prompt needs iteration on real data. Current
  // version is a first draft.
  const system = `You are reviewing pages from FlexSim's user manual to remove redundancy.

You receive ONE topic at a time plus a summary list of topics already classified as CANONICAL. For each new topic, decide:

1. KEEP — this topic introduces material not adequately covered by any existing canonical topic. Output: { "action": "keep" }.
2. MERGE — this topic's content is mostly redundant with an existing canonical topic. Output: { "action": "merge", "into_id": "<canonical-id>", "missing_material": "<one-sentence summary of anything in THIS topic that the canonical DOESN'T cover>" }.
3. DROP — this topic is purely chrome (intros, tutorial scaffolding) and contains no canonical content. Output: { "action": "drop" }.

Your output MUST be a single JSON object. No other text.`;

  const canonicalText = canonicalSummaries.length === 0
    ? '(no canonical topics yet)'
    : canonicalSummaries.map(c => `- ${c.id}: ${c.summary}`).join('\n');

  const user = `Canonical topics so far:
${canonicalText}

---

New topic id: ${topic.id}
Breadcrumb: ${topic.breadcrumb.join(' > ')}
Body (${topic.body.length} chars):

${topic.body.slice(0, 8000)}

---

Classify this topic. Output ONLY the JSON.`;

  const resp = await fetch('https://api.anthropic.com/v1/messages', {
    method: 'POST',
    headers: {
      'content-type': 'application/json',
      'x-api-key': apiKey,
      'anthropic-version': '2023-06-01',
    },
    body: JSON.stringify({
      model: MODEL_ID,
      max_tokens: 600,
      system,
      messages: [{ role: 'user', content: user }],
    }),
  });
  if (!resp.ok) {
    const t = await resp.text();
    throw new Error(`API error ${resp.status}: ${t}`);
  }
  const json = await resp.json();
  const text = (json.content || []).map(c => c.text || '').join('').trim();
  // Pull the JSON block — the model sometimes wraps in code fences.
  const m = text.match(/\{[\s\S]*\}/);
  if (!m) throw new Error('No JSON in response: ' + text);
  return JSON.parse(m[0]);
}

// -----------------------------------------------------------------------
// Main.
// -----------------------------------------------------------------------
// -----------------------------------------------------------------------
// Apply step. Reads the manifest, rewrites topics per the decisions.
// -----------------------------------------------------------------------
function applyManifest() {
  if (!fs.existsSync(manifestPath)) {
    console.error(`No manifest at ${manifestPath}. Run a classify pass first.`);
    process.exit(1);
  }
  const decisions = JSON.parse(fs.readFileSync(manifestPath, 'utf8'));
  const topicsById = new Map();
  for (const t of loadTopics()) topicsById.set(t.id, t);

  let dropped = 0, merged = 0, kept = 0;
  for (const d of decisions) {
    const t = topicsById.get(d.id);
    if (!t) continue;
    if (d.action === 'drop') {
      fs.unlinkSync(t.path);
      dropped++;
    } else if (d.action === 'merge' && d.into_id) {
      // Rewrite as a pointer stub. Keep frontmatter so id resolution still
      // works, but the body just redirects to the canonical topic. The
      // missing_material note (if present) is preserved so a future curator
      // can fold it into the canonical.
      const fmEnd = (() => {
        const raw = fs.readFileSync(t.path, 'utf8');
        if (!raw.startsWith('---')) return 0;
        return raw.indexOf('\n---', 3) + 4;
      })();
      const fm = fs.readFileSync(t.path, 'utf8').slice(0, fmEnd);
      const stub = fm +
        '\n# ' + (t.name || t.id) + ' (merged)\n\n' +
        'This topic was merged into [`' + d.into_id + '`](' + d.into_id + ').\n\n' +
        'See that topic for the canonical text.\n' +
        (d.missing_material
          ? '\n**Missing material noted at merge time** (for a future curator to fold in):\n> ' +
            d.missing_material + '\n'
          : '');
      fs.writeFileSync(t.path, stub, 'utf8');
      merged++;
    } else {
      kept++;
    }
  }
  console.log(`Applied: ${dropped} dropped, ${merged} merged (as pointer stubs), ${kept} kept.`);
  console.log('Next: node tools/build-kb.js   (refresh INDEX.json)');
}

async function main() {
  if (applyMode) {
    applyManifest();
    return;
  }
  console.log(`Manual dedup (model=${MODEL_ID}, dryRun=${dryRun})`);
  const topics = loadTopics();
  console.log(`Loaded ${topics.length} topics`);

  const canonical = [];       // [{ id, summary }]
  const decisions = [];       // [{ id, action, ... }]

  for (let i = 0; i < topics.length; i++) {
    const t = topics[i];
    process.stdout.write(`[${i + 1}/${topics.length}] ${t.id} ... `);

    if (dryRun) {
      console.log('(dry-run, skipping API call)');
      decisions.push({ id: t.id, action: 'keep', dryRun: true });
      canonical.push({ id: t.id, summary: t.name });
      continue;
    }

    try {
      const decision = await classifyTopic(t, canonical);
      console.log(decision.action.toUpperCase() +
        (decision.into_id ? ` → ${decision.into_id}` : ''));
      decisions.push({ id: t.id, ...decision });
      if (decision.action === 'keep') {
        // TODO(tune): the "summary" we feed to subsequent calls should be a
        // 1-sentence description, not just the title. Add a second small
        // LLM call to summarize, or extract the first non-heading line.
        canonical.push({ id: t.id, summary: t.name });
      }
    } catch (e) {
      console.log('ERR ' + e.message);
      decisions.push({ id: t.id, action: 'keep', error: e.message });
      canonical.push({ id: t.id, summary: t.name });
    }
  }

  fs.writeFileSync(manifestPath, JSON.stringify(decisions, null, 2));
  console.log(`\nManifest written: ${path.relative(repoRoot, manifestPath)}`);

  if (dryRun) {
    console.log('(dry-run — no file rewrites performed)');
    return;
  }
  console.log('\nManifest written. Review by hand, then run with --apply.');
}

main().catch(e => { console.error(e); process.exit(1); });
