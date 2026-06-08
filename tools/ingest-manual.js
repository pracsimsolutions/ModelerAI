// Ingest FlexSim's manual into the KB.
//
// Walks <FlexSim install>/manual/**/*.html, strips HTML chrome (head,
// nav, header, script, style, image tags), converts the semantic body to
// markdown, and writes one topic file per HTML page under
// KNOWLEDGE/topics/manual/<relative-path-no-extension>.md.
//
// Skipped (name-based, anywhere in tree):
//   - Tutorials/   — UI walkthrough heavy; conceptual gold is in BestPractices + ModelLogic
//   - Images/      — binary asset dirs
//   - CSS/         — stylesheets
//
// Skipped (specific paths under manual/):
//   - Reference/CodingInFlexSim/CommandReference  — overlaps Commands.xml
//   - Reference/CodingInFlexSim/FlexScriptAPIReference — overlaps the FlexScript API XMLs
//   - Reference/DeveloperAdvancedUser  — DLL/C++ low-level guidance, not for the model AI
//   - Reference/ReleaseNotesAndHistory — version logs
//   - Reference/SystemRequirements      — installation requirements
//
// INCLUDED (added 2026-06-01 after audit revealed Source/Queue/Processor/
// Sink/etc. property docs live under Reference/3DObjects/, NOT the
// FlexScriptAPIReference XMLs):
//   - Reference/3DObjects        — per-3D-object property + event reference
//   - Reference/ProcessFlowObjects — per-activity reference
//   - Reference/PropertiesPanels  — GUI property panel documentation
//   - Reference/Dashboard, GeneralModelSettings, PeopleObjects, CodingInFlexSim/*
//     except the two API-mirror subdirs above.
//
// Output is deterministic given the same input (idempotent). Re-running on
// a new FlexSim version produces a clean overwrite.
//
// Usage:  node tools/ingest-manual.js
//
// Dependency-free (matches build-kb.js + embed.js convention).
'use strict';

const fs   = require('fs');
const path = require('path');

// -----------------------------------------------------------------------
// Paths.
// -----------------------------------------------------------------------
const repoRoot   = path.join(__dirname, '..');
// modules/ModelerAI -> modules -> FlexSim install root
const fxRoot     = path.resolve(repoRoot, '..', '..');
const manualRoot = path.join(fxRoot, 'manual');

const outRoot    = path.join(repoRoot, 'KNOWLEDGE', 'topics', 'manual');

if (!fs.existsSync(manualRoot)) {
  console.error('Could not find FlexSim manual directory at: ' + manualRoot);
  process.exit(1);
}

// Name-based skips (any directory matching these names at any depth).
const SKIP_DIR_NAMES = new Set(['Tutorials', 'Images', 'CSS']);

// Path-based skips (forward-slash, relative to manualRoot). Match if the
// dir's relative path equals or begins with one of these.
const SKIP_PATH_PREFIXES = [
  'Reference/CodingInFlexSim/CommandReference',
  'Reference/CodingInFlexSim/FlexScriptAPIReference',
  'Reference/DeveloperAdvancedUser',
  'Reference/ReleaseNotesAndHistory',
  'Reference/SystemRequirements',
  'FlexSimHC',   // Healthcare module — separate scope
  'FlexSimUI',   // UI customization chapter — separate scope
];

// -----------------------------------------------------------------------
// Walk.
// -----------------------------------------------------------------------
function shouldSkipDir(rel) {
  const norm = rel.replace(/\\/g, '/');
  for (const p of SKIP_PATH_PREFIXES) {
    if (norm === p || norm.startsWith(p + '/')) return true;
  }
  return false;
}

function walkHtml(dir, out, baseDir) {
  const ents = fs.readdirSync(dir, { withFileTypes: true });
  for (const ent of ents) {
    const full = path.join(dir, ent.name);
    if (ent.isDirectory()) {
      if (SKIP_DIR_NAMES.has(ent.name)) continue;
      const rel = path.relative(baseDir, full);
      if (shouldSkipDir(rel)) continue;
      walkHtml(full, out, baseDir);
    } else if (ent.isFile() && ent.name.toLowerCase().endsWith('.html')) {
      // Skip TOC.html and any other top-level navigation files — they're
      // pure link lists with no content.
      if (/^TOC\.html$/i.test(ent.name)) continue;
      out.push(full);
    }
  }
}

// -----------------------------------------------------------------------
// HTML → markdown.
//
// Minimal parser tuned for FlexSim's manual shape. We treat the HTML as a
// loose stream and emit markdown for tags we care about. Everything else
// passes through as text; unknown tags are stripped (but content kept).
// -----------------------------------------------------------------------
function decodeEntities(s) {
  return String(s)
    .replace(/&amp;/g, '&')
    .replace(/&lt;/g, '<')
    .replace(/&gt;/g, '>')
    .replace(/&quot;/g, '"')
    .replace(/&apos;/g, "'")
    .replace(/&nbsp;/g, ' ')
    .replace(/&mdash;/g, '—')
    .replace(/&ndash;/g, '–')
    .replace(/&hellip;/g, '…')
    .replace(/&#x([0-9a-fA-F]+);/g, (_, hex) => String.fromCodePoint(parseInt(hex, 16)))
    .replace(/&#(\d+);/g, (_, dec) => String.fromCodePoint(parseInt(dec, 10)));
}

// Strip everything between <tag>...</tag> (inclusive), case-insensitive.
function stripBlock(html, tag) {
  const re = new RegExp(`<${tag}\\b[^>]*>[\\s\\S]*?</${tag}\\s*>`, 'gi');
  return html.replace(re, '');
}

// Strip self-closing or unpaired single tags entirely (with their attrs).
function stripVoid(html, tag) {
  const re = new RegExp(`<${tag}\\b[^>]*/?>`, 'gi');
  return html.replace(re, '');
}

// Extract <title>...</title> for the topic title fallback.
function extractTitle(html) {
  const m = html.match(/<title>([\s\S]*?)<\/title>/i);
  return m ? decodeEntities(m[1]).trim() : '';
}

// Extract everything inside <body>...</body>; if no body, return the input.
function extractBody(html) {
  const m = html.match(/<body\b[^>]*>([\s\S]*?)<\/body>/i);
  return m ? m[1] : html;
}

// Convert one inline element's HTML to markdown text (recursive on inner).
function convertInline(html) {
  let s = html;
  // Strip image tags entirely — user wants no images.
  s = s.replace(/<img\b[^>]*\/?>/gi, '');
  // <br> → space (we re-emit paragraphs at the block level)
  s = s.replace(/<br\s*\/?>/gi, ' ');
  // Links: <a href="url">text</a> → [text](url). External http(s) URLs kept;
  // internal manual cross-refs rewritten to manual:// stub so we can resolve
  // them later. Strip pure anchor (#section) refs since the section IDs are
  // page-local and meaningless outside the original HTML viewer.
  s = s.replace(/<a\b[^>]*\bhref=(?:"([^"]*)"|'([^']*)')[^>]*>([\s\S]*?)<\/a>/gi,
    (_, q1, q2, inner) => {
      const url = q1 || q2 || '';
      const text = convertInline(inner).trim();
      if (!url || url.startsWith('#')) return text;
      // Heuristic: if it ends with .html, rewrite to manual:// internal ref.
      // The dedup pass (or a later cross-link rewriter) can resolve these.
      const m = /\.html(?:#.*)?$/i.exec(url);
      if (m) return text;  // drop the link; keep the text. Cross-refs add noise.
      return `[${text}](${url})`;
    });
  // Bold / italic.
  s = s.replace(/<(?:strong|b)\b[^>]*>([\s\S]*?)<\/(?:strong|b)\s*>/gi,
    (_, inner) => '**' + convertInline(inner) + '**');
  s = s.replace(/<(?:em|i)\b[^>]*>([\s\S]*?)<\/(?:em|i)\s*>/gi,
    (_, inner) => '*' + convertInline(inner) + '*');
  // Inline code.
  s = s.replace(/<code\b[^>]*>([\s\S]*?)<\/code\s*>/gi,
    (_, inner) => '`' + decodeEntities(inner).replace(/\s+/g, ' ').trim() + '`');
  // Span and other inline wrappers — strip the tag, keep content.
  s = s.replace(/<(?:span|abbr|cite|var|kbd|samp|sub|sup|u|q|small|mark|time)\b[^>]*>([\s\S]*?)<\/(?:span|abbr|cite|var|kbd|samp|sub|sup|u|q|small|mark|time)\s*>/gi,
    (_, inner) => convertInline(inner));
  // Any other tag we don't recognize: strip but keep inner content.
  s = s.replace(/<\/?[a-zA-Z][^>]*>/g, '');
  return decodeEntities(s);
}

// Block-level conversion. Walks the body HTML and emits markdown.
function htmlBodyToMarkdown(html) {
  // 1. Remove non-content blocks entirely.
  let s = html;
  s = stripBlock(s, 'script');
  s = stripBlock(s, 'style');
  s = stripBlock(s, 'noscript');
  s = stripBlock(s, 'iframe');
  s = stripBlock(s, 'header');   // page header with breadcrumb / logo
  s = stripBlock(s, 'nav');      // page navigation
  s = stripBlock(s, 'footer');   // page footer
  s = stripVoid(s, 'meta');
  s = stripVoid(s, 'link');
  s = stripVoid(s, 'img');

  // 2. Normalize whitespace around blocks so the regex passes have clean
  // boundaries. (Doesn't touch text inside <pre>.)
  const PRE_TOKEN_RE = /<pre\b[^>]*>[\s\S]*?<\/pre\s*>/gi;
  const preserved = [];
  s = s.replace(PRE_TOKEN_RE, (m) => {
    preserved.push(m);
    return `@@PRE${preserved.length - 1}@@`;
  });
  s = s.replace(/[ \t\r\n]+/g, ' ');
  s = s.replace(/@@PRE(\d+)@@/g, (_, i) => '\n\n' + preserved[+i] + '\n\n');

  // 3. Headings.
  s = s.replace(/<h1\b[^>]*>([\s\S]*?)<\/h1\s*>/gi,
    (_, inner) => '\n\n# ' + convertInline(inner).trim() + '\n\n');
  s = s.replace(/<h2\b[^>]*>([\s\S]*?)<\/h2\s*>/gi,
    (_, inner) => '\n\n## ' + convertInline(inner).trim() + '\n\n');
  s = s.replace(/<h3\b[^>]*>([\s\S]*?)<\/h3\s*>/gi,
    (_, inner) => '\n\n### ' + convertInline(inner).trim() + '\n\n');
  s = s.replace(/<h4\b[^>]*>([\s\S]*?)<\/h4\s*>/gi,
    (_, inner) => '\n\n#### ' + convertInline(inner).trim() + '\n\n');
  s = s.replace(/<h5\b[^>]*>([\s\S]*?)<\/h5\s*>/gi,
    (_, inner) => '\n\n##### ' + convertInline(inner).trim() + '\n\n');

  // 4. Code blocks: <pre><code class="language-X">...</code></pre> or <pre>...</pre>.
  s = s.replace(/<pre\b[^>]*>([\s\S]*?)<\/pre\s*>/gi, (_, inner) => {
    // Look for a nested code tag with a language class.
    let lang = '';
    const codeMatch = inner.match(/<code\b([^>]*)>([\s\S]*?)<\/code\s*>/i);
    let body;
    if (codeMatch) {
      const attrs = codeMatch[1];
      body = codeMatch[2];
      const langMatch = attrs.match(/class=(?:"|')[^"']*language-([a-zA-Z0-9_-]+)[^"']*(?:"|')/);
      if (langMatch) lang = langMatch[1];
    } else {
      body = inner;
    }
    // Decode entities, preserve internal whitespace, trim outer.
    body = decodeEntities(body.replace(/<\/?[a-zA-Z][^>]*>/g, ''));
    body = body.replace(/^\s*\n/, '').replace(/\n\s*$/, '');
    return '\n\n```' + lang + '\n' + body + '\n```\n\n';
  });

  // 5. Lists. We handle the simple flat case and minimal nesting via
  // recursion through convertInline.
  s = s.replace(/<ul\b[^>]*>([\s\S]*?)<\/ul\s*>/gi, (_, inner) => {
    return '\n\n' + processList(inner, '-') + '\n\n';
  });
  s = s.replace(/<ol\b[^>]*>([\s\S]*?)<\/ol\s*>/gi, (_, inner) => {
    return '\n\n' + processList(inner, '1.') + '\n\n';
  });

  // 6. Tables. Simple grid → markdown table.
  s = s.replace(/<table\b[^>]*>([\s\S]*?)<\/table\s*>/gi, (_, inner) => {
    return '\n\n' + processTable(inner) + '\n\n';
  });

  // 7. Blockquotes.
  s = s.replace(/<blockquote\b[^>]*>([\s\S]*?)<\/blockquote\s*>/gi,
    (_, inner) => '\n\n> ' + convertInline(inner).trim().replace(/\n/g, '\n> ') + '\n\n');

  // 8. <hr>.
  s = s.replace(/<hr\s*\/?>/gi, '\n\n---\n\n');

  // 9. Paragraphs + divs.
  s = s.replace(/<p\b[^>]*>([\s\S]*?)<\/p\s*>/gi,
    (_, inner) => '\n\n' + convertInline(inner).trim() + '\n\n');
  s = s.replace(/<div\b[^>]*>([\s\S]*?)<\/div\s*>/gi,
    (_, inner) => '\n\n' + convertInline(inner).trim() + '\n\n');
  s = s.replace(/<section\b[^>]*>([\s\S]*?)<\/section\s*>/gi,
    (_, inner) => '\n\n' + inner + '\n\n');
  s = s.replace(/<article\b[^>]*>([\s\S]*?)<\/article\s*>/gi,
    (_, inner) => '\n\n' + inner + '\n\n');

  // 10. Any leftover inline-or-unknown tags: convert through inline rules.
  s = convertInline(s);

  // 11. Collapse 3+ newlines to 2; trim outer whitespace.
  s = s.replace(/\n{3,}/g, '\n\n').trim();

  return s;
}

function processList(inner, marker) {
  // Split by <li>...</li>. Tolerate missing closing tags by being permissive.
  const items = [];
  const re = /<li\b[^>]*>([\s\S]*?)(?=<\/li\s*>|<li\b|$)/gi;
  let m;
  while ((m = re.exec(inner)) !== null) {
    let item = m[1];
    // Strip a trailing close-tag.
    item = item.replace(/<\/li\s*>\s*$/i, '');
    // Recurse: an <li> may contain inline + possibly nested lists.
    const itemText = convertInline(item.replace(/<\/?(?:p|div|section)[^>]*>/gi, ' ')).trim();
    items.push(marker + ' ' + itemText.replace(/\s+/g, ' '));
  }
  return items.join('\n');
}

function processTable(inner) {
  // Pull rows.
  const rowRe = /<tr\b[^>]*>([\s\S]*?)<\/tr\s*>/gi;
  const rows = [];
  let rm;
  while ((rm = rowRe.exec(inner)) !== null) {
    const cellRe = /<(?:th|td)\b[^>]*>([\s\S]*?)<\/(?:th|td)\s*>/gi;
    const cells = [];
    let cm;
    while ((cm = cellRe.exec(rm[1])) !== null) {
      cells.push(convertInline(cm[1]).trim().replace(/\|/g, '\\|').replace(/\s+/g, ' '));
    }
    if (cells.length) rows.push(cells);
  }
  if (!rows.length) return '';
  // Treat first row as header. Pad to max width.
  const maxCols = rows.reduce((n, r) => Math.max(n, r.length), 0);
  const pad = (r) => {
    while (r.length < maxCols) r.push('');
    return r;
  };
  const header = pad(rows[0]);
  const sep    = header.map(() => '---');
  const body   = rows.slice(1).map(pad);
  const lines  = [
    '| ' + header.join(' | ') + ' |',
    '| ' + sep.join(' | ')    + ' |',
    ...body.map(r => '| ' + r.join(' | ') + ' |')
  ];
  return lines.join('\n');
}

// -----------------------------------------------------------------------
// Per-file processing.
// -----------------------------------------------------------------------
function deriveTopicId(relPath) {
  // relPath is the path under manual/, e.g.,
  // "BestPractices/WhileBuilding/Containers/Containers.html".
  // Topic id = lowercased path with extension stripped and / → _.
  const noExt = relPath.replace(/\.html$/i, '');
  return noExt.toLowerCase().replace(/[\\/]/g, '_').replace(/[^a-z0-9_-]/g, '_');
}

function deriveBreadcrumb(relPath) {
  const parts = relPath.replace(/\\/g, '/').replace(/\.html$/i, '').split('/');
  return parts;
}

function deriveTags(relPath) {
  const parts = deriveBreadcrumb(relPath);
  // First-level chapter → primary tag. Subsequent dirs → additional.
  const tags = ['manual'];
  for (const p of parts.slice(0, 3)) {
    // Skip the leaf filename if it duplicates the parent dir name.
    if (p.length < 2) continue;
    tags.push(p.toLowerCase());
  }
  return tags;
}

function processFile(htmlPath, relPath) {
  const html = fs.readFileSync(htmlPath, 'utf8');
  const title = extractTitle(html);
  const bodyHtml = extractBody(html);
  const md = htmlBodyToMarkdown(bodyHtml);

  if (!md.trim()) return null;

  const id = deriveTopicId(relPath);
  const breadcrumb = deriveBreadcrumb(relPath);
  const tags = deriveTags(relPath);

  const frontmatter =
`---
id: ${id}
name: ${JSON.stringify(title || breadcrumb[breadcrumb.length - 1])}
kind: manual
breadcrumb: ${JSON.stringify(breadcrumb)}
tags: ${JSON.stringify(tags)}
source: ${JSON.stringify(path.relative(fxRoot, htmlPath).replace(/\\/g, '/'))}
---

`;
  const body = title && !md.startsWith('# ') ? `# ${title}\n\n${md}\n` : md + '\n';
  return { id, relPath, content: frontmatter + body };
}

// -----------------------------------------------------------------------
// Main.
// -----------------------------------------------------------------------
function main() {
  console.log('FlexSim manual ingest');
  console.log('  FlexSim install: ' + fxRoot);
  console.log('  Manual root:     ' + manualRoot);
  console.log('  Output:          ' + outRoot);
  console.log('  Skip names:      ' + Array.from(SKIP_DIR_NAMES).join(', '));
  console.log('  Skip paths:      ' + SKIP_PATH_PREFIXES.join(', '));

  // Wipe the manual output dir so a re-run produces a clean state.
  if (fs.existsSync(outRoot)) {
    rimraf(outRoot);
  }
  fs.mkdirSync(outRoot, { recursive: true });

  const htmlFiles = [];
  walkHtml(manualRoot, htmlFiles, manualRoot);
  console.log(`Found ${htmlFiles.length} HTML files (after skip filters)`);

  let written = 0;
  let skipped = 0;
  let totalBytes = 0;
  for (const htmlPath of htmlFiles) {
    const relPath = path.relative(manualRoot, htmlPath);
    try {
      const result = processFile(htmlPath, relPath);
      if (!result) {
        skipped++;
        continue;
      }
      // Write FLAT — one .md per topic, named by the derived slug. The
      // loader's read_topic() does a flat path lookup so nested directories
      // wouldn't be discoverable. The breadcrumb in frontmatter preserves
      // the original directory structure for display.
      const outPath = path.join(outRoot, result.id + '.md');
      fs.writeFileSync(outPath, result.content, 'utf8');
      written++;
      totalBytes += Buffer.byteLength(result.content, 'utf8');
    } catch (e) {
      console.warn(`  ERR ${relPath}: ${e.message}`);
      skipped++;
    }
  }

  console.log(`\nWrote ${written} topics (skipped ${skipped}), ` +
              `${(totalBytes / 1024).toFixed(1)} KB total`);
  console.log('\nNext step:  node tools/build-kb.js   (regenerates INDEX.json with manual topics)');
  console.log('Optional:   node tools/dedup-manual.js (LLM-assisted dedup pass — see script for cost)');
}

function rimraf(p) {
  if (!fs.existsSync(p)) return;
  for (const ent of fs.readdirSync(p, { withFileTypes: true })) {
    const full = path.join(p, ent.name);
    if (ent.isDirectory()) rimraf(full);
    else fs.unlinkSync(full);
  }
  fs.rmdirSync(p);
}

main();
