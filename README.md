# PracSim ModelerAI

> ⚠️ **Work in progress — no stable version yet.** Active flux: APIs change, files move, features get reworked or removed between commits. No tagged release, no compatibility promise. Treat `main` as a moving target.
>
> Built by an AI assistant (Claude) under direction from Josh at **[Practical Simulation Solutions](https://pracsimsolutions.com)** as a hobby project — "AI builds the AI module that runs another AI." MIT-licensed for anyone who finds it useful, interesting, or fun to fork.
>
> Curious, suggestions, bugs? **<josh@pracsimsolutions.com>** — happy to hear from you.

An AI assistant that lives inside FlexSim 2026's 3D modeler. Chat about your model in plain English; the assistant can read its structure, answer questions, and (with your approval) build and modify it for you.

**[MIT License](LICENSE.txt) — clone, build, modify, ship. No installer, no activation, no telemetry.**

| | |
|---|---|
| **Status** | v0.1 — chat, tool calls under approval, multi-device remote viewer, BYOK. |
| **Providers** | Anthropic Claude (Haiku 4.5 default, Sonnet 4.6, Opus 4.8), OpenAI, Google Gemini, xAI Grok, Mistral; OpenAI-compatible adapter covers Ollama, DeepSeek, etc. |
| **Platform** | Windows-only. FlexSim 2026 required. |
| **Build** | Visual Studio 2022 (C++ desktop workload) + Node.js 20+. |

---

## Architecture in one paragraph

Heavyweight C++ DLL plus a dumb HTML viewer. Provider calls, agent loop, tool implementations, KB, and key storage live in the compiled DLL ([`ModelerAiDLL/`](ModelerAiDLL/)). The chat UI is plain HTML in [`webview/`](webview/) and knows nothing about AI — it just renders what the DLL sends and forwards what you type back.

---

## Setup

Prerequisites: **FlexSim 2026** (full install — ModelerAI reads its bundled API reference and optional manual), **Visual Studio 2022** with the *Desktop development with C++* workload, **Node.js 20+ LTS**, **Git**.

### 1. Get a provider API key

BYOK — you pay the provider directly. Pick one to start; add more later.

| Provider | Cheapest model | Get a key |
|---|---|---|
| **Anthropic** (recommended first try) | Haiku 4.5 ≈ $1 / $5 per Mtok | [console.anthropic.com](https://console.anthropic.com/) |
| **OpenAI** | GPT-4o mini ≈ $0.15 / $0.60 | [platform.openai.com/api-keys](https://platform.openai.com/api-keys) |
| **Google Gemini** | Gemini 2.0 Flash ≈ $0.10 / $0.40 | [aistudio.google.com/apikey](https://aistudio.google.com/apikey) |
| **xAI** (Grok) | Grok 3 Mini ≈ $0.30 / $0.50 | [console.x.ai](https://console.x.ai/) |
| **Mistral** | Mistral Small ≈ $0.20 / $0.60 | [console.mistral.ai](https://console.mistral.ai/) |

Cheap models are fractions of a cent per turn. For pricier models (Opus, GPT-4o, o1), set a $5–$10 cap at the provider.

### 2. Clone into FlexSim's modules folder

```powershell
cd "C:\Program Files\FlexSim 2026\modules"
git clone https://github.com/pracsimsolutions/ModelerAI.git
```

If `git clone` fails with "Access denied," re-run PowerShell as administrator.

### 3. Build the DLL

```powershell
cd ModelerAI\ModelerAiDLL
start ModelerAi.sln
```

In Visual Studio: set platform to **x64**, press **Ctrl + Shift + B**. After 1–3 minutes you'll see "Build: 1 succeeded." `ModelerAI.dll` lands at the repo root.

### 4. Build the knowledge base

Double-click `setup.bat` (or `node tools/build-kb.js` from PowerShell). This regenerates the KB locally from your FlexSim install's API + command XML — nothing FlexSim-derived ships in this repo. Re-run any time FlexSim updates.

### 5. Open the panel

Launch FlexSim, open or create a model, open the **Toolbox**, and double-click **ModelerAI**. The chat panel docks itself; drag the tab wherever you like. It auto-reopens with that model going forward.

### 6. First-launch wizard

The first time the panel opens, it walks you through: welcome → paste API key (encrypted via Windows DPAPI to `%APPDATA%\PRACSIM\ModelerAI\keys.dat`) → optional cost cap (try `warn=$2, stop=$5`) → pick default model → done. Skip it and configure from **Settings → Setup** if you prefer.

Send a test message: *"Hi, can you see my model?"*

---

## Remote viewer (phone / tablet)

In the chat input:

```
/remote on
```

Scan the QR code with a device on the same Wi-Fi. Same chat surface opens there; desktop and mobile stay in sync. `/remote off` to stop, `/remote diag` to inspect. Plain HTTP on LAN — fine on home/office Wi-Fi, **not** appropriate for hostile or guest networks. Don't expose to the public internet.

---

## Slash commands

Type `/` for autocomplete. Highlights:

| Command | What it does |
|---|---|
| `/model <id>` | Switch model. Shortcuts: `haiku`, `sonnet`, `opus`, or full IDs (`gpt-4o`, `gemini-2.5-pro`, `grok-3`). |
| `/effort high` | Extended thinking budget (Sonnet, Opus, o1, o3, Gemini 2.5). Smarter, slower, pricier. |
| `/clear` | Wipe conversation, start fresh. |
| `/cost` | Token usage + cumulative session cost. |
| `/export` | Save conversation as Markdown under `%APPDATA%\PRACSIM\ModelerAI\exports\`. |
| `/tools` | List every tool the agent has. |
| `/rules` | Open your durable user-rules file (re-read every message). |
| `/remote on/off/status/diag/trace` | Multi-device viewer controls. |
| `/help` | Every command, with descriptions. |

---

## Optional: include the FlexSim user manual in the KB

`setup.bat` indexes the API + command reference only. The written user manual is FlexSim IP — not redistributed here, but you can opt in locally:

```powershell
node tools/ingest-manual.js
node tools/build-kb.js
```

Reads `<FlexSim>\manual\**\*.html`, converts to markdown under `KNOWLEDGE\topics\manual\` (gitignored), rebuilds the index. ~600 topics, ~5 MB, a minute or two. Content stays on your machine.

---

## On-disk layout

Everything lives under `%APPDATA%\PRACSIM\ModelerAI\`:

| File | Purpose |
|---|---|
| `keys.dat` | DPAPI-encrypted API keys, bound to your Windows user. |
| `settings.json` | Preferences (model, effort, theme, cost cap). |
| `sessions/` | Saved conversation history. |
| `exports/` | `/export` output. |
| `bridge.log` | UI⇄DLL debug log. Off unless `MODELER_AI_BRIDGE_LOG=1` is set before launching FlexSim. |
| `diag.log` | Remote-viewer diagnostic log. |

Your messages go directly from your machine to the AI provider you configured. PracSim has no server in the data path — no telemetry, no tracking.

---

## Development loop

| You changed | Run | Reload |
|---|---|---|
| C++ under `ModelerAiDLL/` | Ctrl + Shift + B | **Close FlexSim entirely**, then reopen |
| HTML/JS/CSS under `webview/` | `node tools/embed.js` | Reload model (Ctrl + R) |
| FlexScript in `ModelerAI.fsx` | (save the file) | Reload model |
| KB topic under `KNOWLEDGE/topics/` | `node tools/build-kb.js` | No reload — read lazily on next `search_kb` |

Extending ModelerAI? Start with [`AGENTS.md`](AGENTS.md) — build pipeline, source layout, recipes for common extension tasks, non-obvious constraints. Commit / PR conventions live in [`CONTRIBUTING.md`](CONTRIBUTING.md).

---

## License

[MIT](LICENSE.txt). Copyright (c) 2026 Practical Simulation Solutions. Provided "as is," no warranty.

AI output may be wrong. **Review what the AI suggests before applying it to important models**, and verify your provider's terms of service before sending anything sensitive.

Email **<josh@pracsimsolutions.com>** for questions, suggestions, or just to chat. More at [pracsimsolutions.com](https://pracsimsolutions.com).
