# Contributing to PracSim ModelerAI

Thanks for considering a contribution. The project is MIT-licensed, so you're free to fork, modify, and ship — sending fixes back upstream is appreciated but not required.

## Before you start

If you haven't yet, work through the setup in [`README.md`](README.md) first — clone into your FlexSim modules dir, build the DLL, generate the KB index, launch the panel, send a test message. You need a working local build before any meaningful change is testable.

For *how the source is laid out and how to add new functionality without breaking existing assumptions*, read [`AGENTS.md`](AGENTS.md). It has the build pipeline, recipes for the common extension tasks, the gotchas that have bitten people before, and pointers to the diagnostic surfaces.

## Edit / build / reload cycle

| Change | Run | Reload in FlexSim |
|---|---|---|
| C++ in `ModelerAiDLL/` | Build the VS solution | **Close FlexSim fully and reopen** — Windows pins loaded DLLs for the process lifetime, so model-reload alone won't pick up the new build |
| HTML / JS / CSS in `webview/` | `node tools/embed.js` (re-embeds into `ModelerAI.fsx`) | Reload the model (Ctrl+R or close/reopen the panel) |
| FlexScript inside `ModelerAI.fsx` `<data>…</data>` blocks | (manual edit; no build step) | Reload the model |
| KB topic added or edited under `KNOWLEDGE/topics/` | `node tools/build-kb.js` | No reload — the agent reads `INDEX.json` lazily |

If the linker fails with `LNK1104 cannot open file 'ModelerAI.dll'`, FlexSim is running and holding the old DLL. Close FlexSim and rebuild.

## Version marker — bump on every change

The chat viewer renders a tiny build version (e.g. `.1000001`) in the top-right corner. It's a "did the change reach me?" diagnostic — if you edit `webview/index.html` and forget `node tools/embed.js`, or edit C++ and don't rebuild + restart FlexSim, the number in the corner stays the same and you know immediately.

**Bump the version by `.0000001` in the same commit as your change.** So `.1000001` → `.1000002` → `.1000003`. Applies to any source edit that requires a rebuild or re-embed to take effect (C++, HTML, JS, CSS, FlexScript inside the .fsx). Grep `webview/index.html` for `app-version` to find the element, edit the literal, run `node tools/embed.js`. See [`AGENTS.md`](AGENTS.md#version-marker--did-the-change-reach-me) for more detail.

## Code style

- **Conventional commits.** Subjects like `feat(viewer): add X`, `fix(remote): Y`, `docs: Z`, `diag: …`. The history at `git log --oneline` shows the established pattern — match it.
- **C++**: matches the surrounding code. Lowercase namespaces, snake_case for free functions, CamelCase for types, member-suffix `_` for private fields. Headers in their owning directory; no PCH.
- **JavaScript inside `webview/index.html` stays ES5-ish.** No arrow functions, no destructuring, `var` everywhere. CEF's V8 build in some FlexSim versions chokes on newer syntax; this rule prevents surprises. Modern features are fine in `tools/*.js` (those run under your local Node, not in CEF).
- **No emojis in source or commit messages** unless they're already part of a UI string.
- **No comments narrating what the code does** when the names already say it. Add comments for the WHY: invariants, gotchas, hidden constraints. The existing source is a reasonable reference.
- **Don't add backwards-compatibility shims** for code paths no consumer uses anymore. Delete cleanly.

## Filing issues and PRs

Use GitHub Issues and PRs on the repo. Useful things to include:

- For a bug: FlexSim version, ModelerAI commit you're on, what you typed, what the chat surface showed, what `diag.log` (if relevant — see `/remote diag`) shows.
- For a feature: what use case it solves, ideally a sketch of the bridge protocol / slash command / settings shape before you write code.

For non-trivial changes, open an issue first so we can discuss approach before you invest implementation time. Small fixes can go straight to PR.

## Sub-areas — pointers if you're looking for somewhere to help

- **Provider adapters**: [`ModelerAiDLL/provider/`](ModelerAiDLL/provider/) — Anthropic adapter is the reference. OpenAI-compatible adapter handles custom providers. Gemini adapter is deferred.
- **Tool implementations**: [`ModelerAiDLL/tools/`](ModelerAiDLL/tools/) and [`ModelerAiDLL/commands/`](ModelerAiDLL/commands/). The agent invokes these through the bridge.
- **Knowledge base topics**: [`KNOWLEDGE/topics/`](KNOWLEDGE/topics/) — flat markdown files. Adding a topic is purely additive (`node tools/build-kb.js` after).
- **Viewer UI**: [`webview/index.html`](webview/index.html). Single-file viewer; ES5-ish only.
- **Remote viewer**: [`ModelerAiDLL/remote/`](ModelerAiDLL/remote/). cpp-httplib based; serves the same viewer over LAN HTTP.

## License

By contributing, you agree your contribution is offered under the [MIT License](LICENSE.txt) — the same terms as the rest of the project.

## Questions or just want to chat?

Email Josh at <josh@pracsimsolutions.com> — happy to talk through design choices, scope questions, or anything else before you sink time into a PR. More about the project's home at [pracsimsolutions.com](https://pracsimsolutions.com).
