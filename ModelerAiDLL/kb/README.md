# kb/ — Knowledge base subsystem

The KB is how ModelerAI gets smarter over time without re-shipping the DLL. Topic files (markdown) live under `../../KNOWLEDGE/topics/`; the KB layer indexes them, scores relevance per turn, and assembles the matching topics into a "Relevant knowledge" block that the agent appends to the system prompt.

## Design intent

> Before implementing anything in FlexSim, consult the KB. If no topic covers the current situation and the action could mutate the model, ask the user before guessing.

This rule lives in the system prompt that ships with every turn. The model is trained to honor it because the prompt explicitly says so AND because the KB topics in context describe how to do specific things; without a topic, the model knows it doesn't have authoritative guidance.

## Files (planned)

- `kb_index.cpp / .h` — parse + cache `KNOWLEDGE/INDEX.md`
- `kb_loader.cpp / .h` — lazy-load a topic body by id
- `kb_match.cpp / .h` — score topics for relevance vs. the current user message + recent context
- `kb_prompt.cpp / .h` — format the matching topic bodies into a system-prompt block

## INDEX.md format

```markdown
| id | name | appliesWhen (summary) | tags | last_verified |
|---|---|---|---|---|
| spawn-flowitem-pattern | How to spawn... | source / item creation | flowitem,source | 2026-05-23 |
```

## Topic file format

See `KNOWLEDGE/template.md` for the canonical frontmatter + body template. Frontmatter fields: `id`, `name`, `appliesWhen`, `last_verified`, `tags`. Body is markdown with optional `# Example` and `# See also` sections.

## Matching algorithm (v0)

Deterministic. NO embeddings, NO vector DB. The match score is the sum of:

- Keyword overlap between user message and topic `name` + `tags`
- Substring matches between recent conversation context and topic `appliesWhen` lines
- Recency bonus for topics with `last_verified` ≤ 90 days

Top-K topics (K = 3 initially) get loaded. If no topic exceeds a minimum score threshold, no KB block is included — the system prompt instead carries the "ask the user before guessing" rule unmodified.

## Growing the KB

User (or future contributors) adds a topic by:
1. Drop a new `.md` in `KNOWLEDGE/topics/` using `template.md`.
2. Run the index-rebuild script (or edit `INDEX.md` by hand).
3. Commit. Next module load picks it up — no DLL rebuild required.

That's the whole point. Each new topic = the AI gets smarter about one more thing.
