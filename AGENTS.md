# AGENTS.md — guide for anyone (human or AI) building on this module

> If you're an AI assistant working in this repo, read this file first. If you're a human picking this up to keep extending it, same. This file documents how the pieces fit together and how to add new functionality without breaking the existing assumptions.
>
> Keep this file up to date as you make changes. If you introduce a new convention, gotcha, or extension pattern, add it here.

## The shape of the thing

```
ModelerAI/
├── ModelerAI.fsx               FlexSim module file. FlexSim reads this on load.
│                               Contains the embedded HTML viewer (re-generated
│                               by tools/embed.js) PLUS FlexScript glue.
├── ModelerAI.dll               Built by Visual Studio from ModelerAiDLL/.
│                               (gitignored — every contributor builds locally)
├── webview/index.html          The actual source of the HTML viewer. Edit here,
│                               run embed.js to push into .fsx.
├── ModelerAiDLL/               C++ DLL project. Owns 95% of business logic.
│   ├── ModelerAi.sln           VS solution. Build x64/Debug or Release.
│   ├── bridge/                 JSON-envelope protocol between DLL and viewer.
│   ├── agent/                  Agent loop, slash commands, prompt, user rules.
│   ├── provider/               Anthropic + OpenAI-compatible adapters.
│   ├── tools/                  Tool implementations the agent can call.
│   ├── commands/               The modelerai_* curated command library.
│   ├── kb/                     Knowledge base loader + BM25 search.
│   ├── remote/                 LAN remote viewer (cpp-httplib HTTP server).
│   ├── settings/               Effective-settings resolution (global+project).
│   ├── keystore/               DPAPI-encrypted API key storage.
│   ├── mainthread/             Worker→main-thread queue for FlexSim SDK calls.
│   └── third_party/            Vendored: nlohmann/json, cpp-httplib.
├── KNOWLEDGE/
│   ├── topics/classes/         (auto by build-kb.js; tracked in repo) Per-class
│   │                            markdown extracted from FlexSim's API XMLs.
│   ├── topics/commands/        (auto by build-kb.js; tracked in repo) Per-command
│   │                            markdown extracted from Commands.xml.
│   ├── topics/manual/          (auto by ingest-manual.js; GITIGNORED — FlexSim
│   │                            written-manual IP; opt-in per user, local only)
│   ├── topics/<other>/         (hand-written, e.g. concepts/, patterns/, etc.)
│   ├── catalog/                (auto by build-kb.js; tracked) Always-on index pages.
│   ├── schemas/                (auto at runtime by modelerai_extract_class_schema)
│   └── INDEX.json              BM25 index (gitignored; build via tools/build-kb.js).
├── setup.bat                   One-click first-run script for non-coders: runs
│                                node tools/build-kb.js with friendly error messages.
├── tools/
│   ├── embed.js                Re-embeds webview/index.html into ModelerAI.fsx.
│   ├── build-kb.js             Generates topics/classes/, topics/commands/, catalog/,
│   │                            and INDEX.json from FlexSim's help XMLs.
│   └── ingest-manual.js        OPT-IN. Scrapes FlexSim's written user manual under
│                                <FlexSim>/manual into topics/manual/. Not run by
│                                default — manual content is FlexSim IP that we
│                                don't redistribute. User runs it locally if they
│                                want richer KB coverage.
├── docs/
│   ├── architecture/           Locked design decisions (dated; don't rewrite).
│   ├── specs/                  Feature specs (dated; don't rewrite).
│   └── superpowers/            Implementation plans + spec docs from the
│                               Superpowers brainstorming workflow.
└── README.md                   First-time setup. Read for the build prerequisites.
```

## The build pipeline (this is the non-obvious part)

Three artifacts get produced from source. Each has its own pipeline:

```
                 ┌────────────────────────────────────────┐
                 │ FlexSim loads ModelerAI.fsx on startup │
                 │ → reads embedded HTML, calls DLL       │
                 └────────────────────────────────────────┘
                                  ▲           ▲
                                  │           │
                                  │           │
   ┌──────────────────────────────┘           └────────────┐
   │                                                       │
   │  webview/index.html                                   │  ModelerAiDLL/*.cpp
   │           │                                           │           │
   │           │  node tools/embed.js                      │           │  Visual Studio
   │           ▼                                           │           ▼
   │  ModelerAI.fsx  ←─── (HTML embedded as XML-escaped    │  ModelerAI.dll
   │   (committed)        data inside FlexScript nodes)    │   (gitignored, you build)
   │
   │  KNOWLEDGE/topics/*.md
   │           │
   │           │  node tools/build-kb.js
   │           ▼
   │  KNOWLEDGE/INDEX.json  (gitignored; agent reads at runtime)
```

**Key thing to internalize:** `ModelerAI.fsx` is **committed and has the viewer baked in.** If you change `webview/index.html` and forget to run `node tools/embed.js`, your change won't appear in FlexSim — the .fsx is the file FlexSim reads.

### Edit / build / reload cycle by change type

| Change to | Run | Reload how |
|---|---|---|
| `ModelerAiDLL/**/*.{cpp,h}` | Build the VS solution (Ctrl+Shift+B) | **Close FlexSim fully and reopen.** Windows pins loaded DLLs for the process lifetime — model-reload alone won't pick up new code. |
| `webview/index.html` | `node tools/embed.js` | Reload the model in FlexSim (Ctrl+R) or close+reopen the panel. |
| FlexScript inside `ModelerAI.fsx` (the `<data>...</data>` blocks around the embedded HTML) | (manual edit, no build step) | Reload the model. |
| `KNOWLEDGE/topics/*.md` | `node tools/build-kb.js` | No reload — the agent reads `INDEX.json` lazily on each `search_kb` call. |

If the linker fails with `LNK1104 cannot open file 'ModelerAI.dll'`, FlexSim is running and holding the old DLL. Close FlexSim and rebuild.

### Version marker — "did the change reach me?"

The chat viewer renders a small build version (e.g. `.1000001`) in the top-right corner. It exists for one purpose: at a glance, confirm the running UI is the build you just made. If you edit `webview/index.html` and forget `node tools/embed.js`, or edit C++ and forget to rebuild + restart FlexSim, the corner number stays at the old value — you see immediately.

**Rule:** every source change (C++, HTML, JS, CSS, FlexScript, anything that requires a rebuild or re-embed to take effect) bumps the version by `.0000001`. So `.1000001` → `.1000002` → `.1000003` etc. Bump in the same commit as the change.

The version lives in `webview/index.html` (grep `app-version`). It's deliberately not nested inside `#tabbar` or `#pane-settings` so it stays visible in both host mode and `body.phone-mode`. Bumping it counts as an HTML change → run `node tools/embed.js` to push it into `ModelerAI.fsx` so the FlexSim local panel picks it up too.

## How information flows at runtime

```
User types in chat
  │
  ▼
webview/index.html → builds JSON envelope { t: "user_message", id, p: {...} }
  │
  ▼
dispatchEnvelope() → routes to local CEF (fireFlexsimEvent) or remote HTTP (fetch /api/send)
  │
  ▼  (local path)                          (remote path)
ModelerAi_bridgeSend                       /api/send handler in remote_server.cpp
  │                                         │
  └────────────────┬───────────────────────┘
                   ▼
        bridge::handleEnvelope(envelope, requesting_sid)
                   │
                   ▼
        Agent::startTurn → worker thread → provider stream
                   │
                   ▼
        provider emits chunks, tool calls → bridge::pushEvent or pushEventTo
                   │
                   ▼
        per-subscriber queue → drained by /api/poll (remote) or modelerAiBridgePoll (local)
                   │
                   ▼
        viewer renders chunks
```

The single chokepoint **on the way out** is `bridge::pushEvent` (broadcast) or `bridge::pushEventTo(sid, ...)` (targeted to one subscriber). The single chokepoint **on the way in** is `bridge::handleEnvelope`.

The single chokepoint **on the JS send side** is `dispatchEnvelope()` in `webview/index.html`. **Never call `fireFlexsimEvent('modelerAiSend', ...)` directly** — it bypasses the IS_REMOTE branch and silently no-ops in a real browser. This bug ate hours; the rule is "if you're sending an envelope, call `dispatchEnvelope`."

## Common extension recipes

### Add a curated `modelerai_*` command (the AI's main tool surface)

1. Add `ModelerAiDLL/commands/<name>.cpp` + `<name>.h`. Mirror the pattern in `ping.cpp` — `modelerai_export Variant ModelerAi_<Name>(FLEXSIMINTERFACE)`, thread-local result buffer, try-catch.
2. Add a sibling binding in `ModelerAI.fsx` under the bridge bindings:
   ```xml
   <node f="1000042" dt="2">
     <name>modelerai_<name></name>
     <data>dll:"module:ModelerAI" func:"ModelerAi_<Name>"</data>
   </node>
   ```
3. Add the `.cpp/.h` to `module.vcxproj` and `module.vcxproj.filters`.
4. Hand-write `KNOWLEDGE/topics/modelerai/<name>.md` (parameter contract, examples, return shape).
5. Add a row to `KNOWLEDGE/catalog/modelerai-commands.md`.
6. Run `node tools/build-kb.js` so `search_kb` can find the new topic.
7. Build + reload.

The AI invokes these via the `modelerai_call` bridge tool. The argument schema becomes "name + JSON args"; the curated command parses + executes safely.

### Add a slash command (e.g. `/foo`)

1. In `ModelerAiDLL/agent/slash.cpp`, add an entry to `commandCatalog()` via `makeCmd("foo", aliases, brief, details, usage, argSpec, has_args)`.
2. If the command needs args, define an `ArgSpec` with `placeholder` + `choices`.
3. Add a dispatch branch in `dispatchSlash` that handles `cmd == "foo"`.
4. For complex slash commands (multiple subcommands, like `/remote`), extract a `slash_<name>.cpp/.h` module and a `dispatch<Name>(agent, turnId, args)` function. Pattern: see `slash_remote.cpp`.

### Add a tool the agent can call

1. New file under `ModelerAiDLL/tools/read/` or `tools/write/`. Subclass `ITool` from `tool_base.h`.
2. Implement `name()`, `description()`, `params_schema()`, `category()` (Read / Mutating / etc), `run(args_json)`.
3. Register in `ModelerAiDLL/tools/tool_registry.cpp`.
4. Add to `module.vcxproj` + filters.
5. If the tool touches FlexSim SDK state, **wrap SDK calls in `mainthread::runAndWait`** — providers run on worker threads; SDK calls from there will crash or corrupt state.

### Add a KB topic (no DLL rebuild needed)

The KB has three layers and each is added differently:

- **Hand-written topics** (the only kind YOU add by hand). Drop a `.md` file under a NEW or EXISTING area dir — e.g. `KNOWLEDGE/topics/concepts/<topic>.md`, `KNOWLEDGE/topics/patterns/<topic>.md`, or a domain dir like `KNOWLEDGE/topics/modelerai/<command>.md`. Frontmatter is optional; the body is what gets indexed. **Do NOT put hand-written topics under `topics/classes/`, `topics/commands/`, `topics/manual/`, or `catalog/`** — those are wiped on each `build-kb.js` run.

- **Auto class/command topics** (regenerated each run). Edited indirectly — they come from FlexSim's `help/*.xml`. If FlexSim ships an updated help XML and you want the change reflected, just re-run `node tools/build-kb.js`.

- **Auto manual topics** (opt-in, local only). Generated by `node tools/ingest-manual.js` from `<FlexSim install>/manual/**/*.html`. **Don't commit `topics/manual/`** — it's FlexSim-derived prose and we don't redistribute it through the repo. Each user runs `ingest-manual.js` against their own FlexSim install if they want richer KB.

After adding or editing any topic file, run `node tools/build-kb.js` to rebuild `INDEX.json`. The agent picks up new topics on the next `search_kb` call — no DLL rebuild, no FlexSim reload.

### Add a provider

The repo currently ships first-class adapters for **Anthropic, OpenAI, Google Gemini, xAI Grok, and Mistral**, plus an OpenAI-compatible adapter for everything that speaks the Chat Completions wire format (Ollama, DeepSeek, custom local endpoints, etc.). Two paths for adding a new one:

**If the provider speaks OpenAI-Chat-Completions natively** (most do), the easiest path is the `BuiltinChatCompletions` adapter under `ModelerAiDLL/provider/builtin_chat.{h,cpp}`. Look at how `openAiBuiltinConfig()`, `xaiBuiltinConfig()`, and `mistralBuiltinConfig()` are defined in `bootstrap.cpp` — each is a `BuiltinChatConfig` populated with provider id, display name, base URL, env-var fallback, default model id, and a curated `ModelInfo` list. Add yours alongside them and register with `bootstrap::initialize()`. No new .cpp/.h file needed.

**If the provider has its own wire format** (Anthropic / Gemini are the precedents), do a full custom adapter:

1. New file under `ModelerAiDLL/provider/<name>.cpp/.h` implementing `IProvider` from `provider.h`.
2. Register in `bootstrap::initialize()` so the agent loop can find it.
3. Add to `module.vcxproj` + filters (Solution Explorer → Add → Existing Item does both).
4. Add catalog choices in `slash.cpp` `modelArgSpec()` for each model the provider exposes.

In both cases, when populating each `ModelInfo`, remember the struct has an `accepts_temperature` field — if the provider rejects `temperature` (Anthropic Opus 4.8 does), set it to `false`. Otherwise default to `true`.

### Modify the viewer

1. Edit `webview/index.html`.
2. **JS must stay ES5-ish.** No arrow functions, no destructuring, no `const`/`let`, `var` everywhere. CEF's V8 in some FlexSim versions chokes on newer syntax. The constraint applies only inside `webview/`; `tools/*.js` runs under your local Node and can use anything.
3. Run `node tools/embed.js` to push into `ModelerAI.fsx`.
4. Reload the model in FlexSim.

## Non-obvious constraints and gotchas

- **FlexSim SDK is single-threaded.** Anything that touches `Object`, `Group`, `tree`, etc. must run on the FlexSim main thread. Worker threads (agent loop, provider stream, HTTP server workers) use `mainthread::runAndWait` to marshal SDK calls.
- **Windows DLLs are process-pinned.** After rebuilding `ModelerAI.dll`, you MUST close FlexSim entirely and reopen — reload-the-model doesn't reload the DLL.
- **The viewer's JS is wrapped in an IIFE.** State like `IS_REMOTE`, `REMOTE_TOKEN`, `sendEnvelope` is NOT in window scope. Debug via `window.__mraiDiag` (exposes IS_REMOTE, REMOTE_SID, recent sends, and `__mraiDiag.send('text')` for testing the network path).
- **The mock `fireFlexsimEvent` shim** in the viewer is a no-op for non-CEF browsers (mobile, desktop Chrome). Any send that calls it directly instead of going through `dispatchEnvelope` will silently fail in remote mode.
- **The `mraisid` cookie isn't reliable** on plain-HTTP + IP-address origins (Safari especially drops it). The viewer uses an `X-Mraisid` header generated client-side and persisted in localStorage as the primary identity channel; cookie is a fallback.
- **DLL exports must be plain C++ functions (NOT `extern "C"`).** FlexSim's binding layer locates them by mangled name. See `abi.cpp` — the `extern "C"` wrap was tried once and broke the call.
- **`#include "module.h"` must precede `#include "third_party/json.h"`** because FlexSim's `BasicMacros.h` defines macros (`small`, `o`, `ip`) that collide with json.hpp identifiers. The json.h wrapper undefs them defensively.
- **The persona prompt lives in the DLL** (`agent/prompt.cpp`), not the viewer. It used to be hidden as "IP protection"; under MIT that's not a constraint, but keeping it in the DLL is still cleaner — the viewer source stays small.

## Diagnostic surface

When something doesn't work, these are the places to look:

| Surface | What it shows | How to enable / read |
|---|---|---|
| **FlexSim Output Console** | `pt()` calls from main-thread C++. Includes `[ModelerAI] handleEnvelope: received` and similar. | Visible by default. View → Output Console. |
| **`%APPDATA%/PRACSIM/ModelerAI/diag.log`** | Thread-safe diagnostic log for the remote subsystem and worker-thread code. Survives worker-thread context where `pt()` is unreliable. INFO is always on; TRACE is gated. | Tail with `Get-Content -Wait` in PowerShell. Toggle TRACE via `/remote trace` or set env var `MODELER_AI_REMOTE_TRACE=1` before launching FlexSim. |
| **`%APPDATA%/PRACSIM/ModelerAI/bridge.log`** | Pretty-printed JSON of every envelope crossing the JS↔DLL boundary. | Enable with env var `MODELER_AI_BRIDGE_LOG=1` before launching FlexSim. |
| **`/remote diag` slash command** | Live state of the remote server: subscribers, queue depths, main-thread queue, recent envelopes per subscriber. | Just type it in the local chat. |
| **`window.__mraiDiag` in browser DevTools** | IS_REMOTE flag, REMOTE_SID, recent sends ring, last fetch status. Plus `__mraiDiag.send('text')` to bypass the input UI and test the network path. | Press F12 in the remote-viewer browser tab. |
| **DebugView (SysInternals)** | `OutputDebugStringA` mirror of `diag.log`. Live view; survives FlexSim console scroll limits. | Run `dbgview64.exe` from SysInternals while FlexSim is open. |

## Where state lives on disk

Everything ModelerAI persists is under `%APPDATA%/PRACSIM/ModelerAI/`:

- `keys.dat` — DPAPI-encrypted API keys (Win32 user-bound; can't be moved between accounts).
- `settings.json` — global user preferences.
- `sessions/` — per-session saved conversation history.
- `bridge.log` — optional envelope trace (enabled via env var).
- `diag.log` — always-on remote-viewer diagnostic log.
- `exports/` — `/export` slash command output.

Project-scoped settings (per-model) live inside the FlexSim model's tree itself — not on disk.

## Conventions

- **Conventional commits**: `feat(scope): ...`, `fix(remote): ...`, `docs: ...`, `diag: ...`. The history shows the established pattern.
- **C++ headers**: every file starts with
  ```cpp
  // Copyright (c) 2026 Practical Simulation Solutions.
  // Licensed under the MIT License — see LICENSE.txt at the repo root.
  ```
- **Namespaces**: `ModelerAi::<area>` (note lowercase `i`).
- **No comments narrating what the code does** when names already say it. Add comments for WHY: invariants, gotchas, hidden constraints, "we tried X, it broke because Y."
- **No emojis in source or commit messages** unless they're already part of a UI string.
- **No backwards-compatibility shims** for code paths no consumer uses. Delete cleanly.
- **No telemetry, ever**, without explicit user opt-in. ModelerAI never phones home.

## Deep references

- [`README.md`](README.md) — first-time setup, prerequisites, install path, onboarding wizard walkthrough.
- [`CONTRIBUTING.md`](CONTRIBUTING.md) — style, commit conventions, sub-area pointers.
- [`docs/architecture/2026-05-23-modeler-ai-scaffolding.md`](docs/architecture/2026-05-23-modeler-ai-scaffolding.md) — original scaffolding decisions, DLL/viewer split rationale, BYOK posture, bridge protocol shape. **Read this before any non-trivial change.**
- [`docs/architecture/2026-06-01-modelerai-commands-library.md`](docs/architecture/2026-06-01-modelerai-commands-library.md) — current commands-as-API library design.
- [`docs/superpowers/specs/2026-06-05-remote-viewer-design.md`](docs/superpowers/specs/2026-06-05-remote-viewer-design.md) and [`docs/superpowers/specs/2026-06-05-remote-viewer-audit.md`](docs/superpowers/specs/2026-06-05-remote-viewer-audit.md) — full design + post-implementation audit of the multi-device remote viewer.

## When you finish a non-trivial change

Update this file. Specifically:

- If you added a new extension recipe pattern, add it under "Common extension recipes."
- If you hit a non-obvious constraint and worked around it, add it under "Non-obvious constraints and gotchas" so the next person doesn't rediscover it.
- If you added a new diagnostic surface or env-var toggle, add it under "Diagnostic surface."
- If you changed where something lives on disk, update "Where state lives on disk."

The goal: this file should be the place an AI session can read once and have enough context to make a safe, well-shaped change without rediscovering everything by grep.
