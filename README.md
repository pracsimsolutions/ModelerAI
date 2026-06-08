# PracSim ModelerAI

> ⚠️ **WORK IN PROGRESS — NO STABLE VERSION YET.** Every part of this repo is in active flux. APIs change, files move, features get reworked or removed between commits. There is no tagged release, no documented stable surface, and no compatibility promise. If you clone and build right now, expect rough edges and breaking changes. Treat the `main` branch as a moving target.
>
> **About this project** — ModelerAI was built by an AI assistant (Claude) working under direction from Josh at **[Practical Simulation Solutions](https://pracsimsolutions.com)**, as a hobby project to see how far the "AI builds the AI module that runs another AI" idea could be taken. Released as MIT-licensed open source for anyone who finds it useful, interesting, or fun to fork.
>
> **Curious? Have a suggestion? Found a bug?** Email Josh at **<josh@pracsimsolutions.com>** — happy to hear from you.

An AI assistant that lives inside FlexSim 2026's 3D modeler. Chat about your model in plain English; the assistant can read its structure, answer questions, and (with your approval) build and modify it for you.

**Released under the [MIT License](LICENSE.txt) — clone, build, modify, ship. No installer, no activation, no telemetry.**

| | |
|---|---|
| **Status** | Working v0.1 — chat, tool calls under approval, multi-device remote viewer, BYOK to Anthropic / OpenAI / Gemini / xAI / Mistral. |
| **License** | MIT |
| **Provider** | Anthropic Claude (Haiku 4.5 default, Sonnet 4.6, Opus 4.8). First-class adapters for OpenAI, Google Gemini, xAI Grok, Mistral; OpenAI-compatible custom-provider adapter for everything else (Ollama, DeepSeek, etc.). |
| **Platform** | Windows-only (relies on Win32 DPAPI for key storage and WinHTTP for streaming). FlexSim 2026 required. |
| **Build** | Visual Studio 2022 (C++ desktop workload) + Node.js 20+. |

---

## Is this for me?

This walkthrough is written assuming you've used FlexSim before but are not a daily software developer. If you've never touched Git, never opened Visual Studio, never typed in PowerShell — that's fine. Each step explains what's happening and what success looks like.

You'll get the most out of ModelerAI if you:
- Use **FlexSim 2026** for actual modeling work.
- Are comfortable installing Windows applications (you'll install three).
- Are willing to read a screen of plain text now and then.
- Have **about an hour** for first-time setup.

You don't need to know C++, JavaScript, or anything about AI. The build step is "press a button in Visual Studio." There is no command-line wizardry.

---

## What you're going to do, in plain English

1. Install some free Windows software (about 20 minutes).
2. Get a free or low-cost AI account from one provider and copy its key.
3. Download the ModelerAI source code into FlexSim's modules folder.
4. Press a button in Visual Studio to compile it (about 5 minutes).
5. Open FlexSim, type one command in a panel called the FlexScript Console, and start chatting with your model.

When you finish, the chat will look like a normal AI chat window docked inside FlexSim. You can ask it questions about your model, and it can build or change things for you with your approval.

---

## Architecture in one paragraph (skip if you're not curious)

ModelerAI is a heavyweight C++ DLL plus a dumb HTML viewer. All the smart stuff — talking to AI providers, the agent loop, tool implementations, the knowledge base, your API key storage — lives in a compiled DLL ([`ModelerAiDLL/`](ModelerAiDLL/)). The chat window itself is plain HTML in [`webview/`](webview/), and it doesn't know anything about how AI works — it just shows you what the DLL sends and forwards what you type back. The split keeps the rendering layer auditable and the bridge protocol clean.

---

## Setup — full step-by-step from nothing to chatting

### Step 1 — Install the free Windows software you'll need

Install these in any order. Skip any you already have.

- **FlexSim 2026** — get it from [flexsim.com](https://www.flexsim.com/) and install it. You need a working FlexSim license to use FlexSim itself; ModelerAI doesn't get in the middle of that. **Use the full / default install** — ModelerAI reads FlexSim's bundled API reference and (optionally) the user manual from inside the install directory to teach the AI about FlexSim, so don't strip those out. If you already installed and aren't sure whether the manual came along, check whether `C:\Program Files\FlexSim 2026\manual\` exists; if it does, you're set.
- **Visual Studio 2022 — Community Edition** is free. Get it from [visualstudio.microsoft.com/vs/community](https://visualstudio.microsoft.com/vs/community/).
  - When the installer asks **which workloads to install**, tick **"Desktop development with C++"**. That's the box you need. Don't tick Visual Studio Code — that's a different smaller program; we need the full Visual Studio 2022.
  - The C++ workload is ~10 GB. Make sure you have room.
- **Node.js (version 20 or newer)** — get it from [nodejs.org](https://nodejs.org/). Pick the LTS (long-term support) installer. Accept all the defaults during install.
- **Git** — get it from [git-scm.com/download/win](https://git-scm.com/download/win). Accept all defaults during install.

After installing these, **restart your computer once.** This isn't always strictly necessary, but it makes sure the new programs are visible from PowerShell, which we'll use in Step 3.

### Step 2 — Get an AI provider API key

ModelerAI is "bring your own key" — you pay the AI provider directly for usage, and your key stays on your computer. PracSim has nothing to do with your bill.

Pick **one** provider to start. You can add more later.

| Provider | Cheapest model | Where to get a key |
|---|---|---|
| **Anthropic** (default — recommended for first try) | Haiku 4.5 ≈ $1 input / $5 output per million tokens | [console.anthropic.com](https://console.anthropic.com/) → "API keys" → "Create key" |
| **OpenAI** | GPT-4o mini ≈ $0.15 / $0.60 | [platform.openai.com/api-keys](https://platform.openai.com/api-keys) |
| **Google Gemini** | Gemini 2.0 Flash ≈ $0.10 / $0.40 | [aistudio.google.com/apikey](https://aistudio.google.com/apikey) |
| **xAI** (Grok) | Grok 3 Mini ≈ $0.30 / $0.50 | [console.x.ai](https://console.x.ai/) |
| **Mistral** | Mistral Small ≈ $0.20 / $0.60 | [console.mistral.ai](https://console.mistral.ai/) |

A typical conversation with the cheap models is fractions of a cent. The pricier ones (Opus 4.8, GPT-4o, o1) can run a few dollars per long session — recommended to set a $5–$10 cap at the provider before you start, just for peace of mind.

Most providers ask you to load $5 or $10 of credit before they activate the key. That's normal. You're not subscribed to anything; you're just funding a usage account.

When the provider shows you the key, **copy it now and save it somewhere safe** (a password manager, or a plain text file you'll delete in five minutes). Most providers only show you the key once.

### Step 3 — Download the ModelerAI source code

ModelerAI's source code lives on GitHub. We're going to copy it onto your computer using a program called Git. This is called *cloning* a repo.

Open **PowerShell**. (Hit Windows key, type "powershell", hit Enter.) A black window opens. Don't worry about it — we'll type three short commands.

Type or paste this line and press Enter:

```powershell
cd "C:\Program Files\FlexSim 2026\modules"
```

This moves PowerShell to the folder where FlexSim looks for installed modules. Nothing visible will happen — it'll just show a new prompt.

Then this:

```powershell
git clone https://github.com/pracsimsolutions/ModelerAI.git
```

You'll see Git print a bunch of text about "Cloning..." and "Receiving objects..." That's normal — it's downloading the source code. Takes about 30 seconds.

Then:

```powershell
cd ModelerAI
```

You're now inside the ModelerAI folder.

**A sanity check:** type `ls` (lowercase L, lowercase S) and press Enter. You should see a list including `ModelerAI.fsx`, `ModelerAiDLL`, `webview`, `KNOWLEDGE`, and others. If you see those, you're good.

**If PowerShell says** "Access denied" **or** "permission denied" **when running** `git clone`: the `C:\Program Files\FlexSim 2026\modules` folder needs your user to be able to write to it. On a default FlexSim install it's already owned by you, but some installs are stricter. Right-click PowerShell in the Start menu and choose "Run as administrator", then redo the three commands above.

### Step 4 — Build the DLL in Visual Studio

This is the part that turns the C++ source code into a `.dll` file FlexSim can load.

In PowerShell, type:

```powershell
cd ModelerAiDLL
start ModelerAi.sln
```

That opens the Visual Studio solution. Visual Studio takes 15–30 seconds to load the first time.

Once Visual Studio is open:

1. **Look at the top toolbar.** There's a dropdown that says either "x86" or "x64". Make sure it says **x64**.
2. Press **Ctrl + Shift + B** to build. (Or use the menu: Build → Build Solution.)
3. Watch the bottom-left status bar. You'll see "Build started..." and then a progress indicator.
4. After 1–3 minutes, the bottom-left should say **"Build: 1 succeeded, 0 failed."** That's the green light.

**A new ModelerAI.dll file is now sitting at the top of the ModelerAI folder.** That's what FlexSim will load.

**If you see a giant red squiggly mess in the editor:** that's IntelliSense, Visual Studio's live code-analysis. It often shows false errors on first open while it's still indexing. Look at the Build output (bottom panel) — if it says "Build: 1 succeeded," you're fine. Ignore the squigglies.

**If the build fails with** `LNK1104: cannot open file 'ModelerAI.dll'`: FlexSim is currently running and has the previous ModelerAI.dll loaded, so the linker can't overwrite it. Close FlexSim entirely (not just the model — the whole application), then press Ctrl + Shift + B again.

**If the build fails with** `cannot open source file BasicMacros.h`: the source code isn't inside a FlexSim install. Make sure your folder really is `C:\Program Files\FlexSim 2026\modules\ModelerAI\` and not somewhere else.

If something else goes wrong, [email Josh](mailto:josh@pracsimsolutions.com) — happy to help.

### Step 5 — Build the knowledge base (double-click `setup.bat`)

The AI uses a search index over a folder of FlexSim-reference markdown topics to look things up. The class + command topics are **auto-generated from your local FlexSim install's API reference XMLs** — they don't ship in this repo, and nothing FlexSim-owned gets redistributed through it. You build them locally, once, with a single double-click.

In File Explorer, navigate to the `ModelerAI` folder you cloned (e.g. `C:\Program Files\FlexSim 2026\modules\ModelerAI\`). **Double-click `setup.bat`.** A black window appears, prints a few status lines, and finishes with "Done. Knowledge base ready." Press a key to close it.

What it just did:
- Read every FlexScript class definition from `<FlexSim>\help\FlexScriptAPIReference\**\*.xml` and wrote one markdown topic per class to `KNOWLEDGE\topics\classes\`.
- Read every FlexScript command from `<FlexSim>\help\CommandReference\Commands.xml` and wrote one markdown topic per command to `KNOWLEDGE\topics\commands\`.
- Built a BM25 search index over all of the above at `KNOWLEDGE\INDEX.json`.

You don't need to look at any of these files; the DLL reads them. Re-run `setup.bat` any time FlexSim updates (new version, new modules, etc.) — it's idempotent.

If you'd rather run it from PowerShell:
```powershell
node tools/build-kb.js
```
Same result.

### Step 6 — Open FlexSim and start the panel

1. Launch **FlexSim 2026** the normal way.
2. Open or create any model — even a brand-new empty one is fine for a first test.
3. From the menu bar, go to **View → Other → FlexScript Console** (the menu name might be slightly different in your FlexSim version; look for "FlexScript Console" anywhere in View / Tools / Window).
4. A small window appears with a text area. Click in it, type:
   ```
   addmodelerai
   ```
   ...and press **Enter**. (Just the word, no slash, no parentheses.)
5. A new docked panel appears with the ModelerAI chat interface. You can drag its tab to dock it wherever you like (next to the 3D view, full-width across the bottom, etc.).

Once you've run `addmodelerai` on a model, the chat panel **automatically reopens** every time you load that model. You only need to type the command once per model.

### Step 7 — First-launch onboarding wizard

The very first time the panel opens, it walks you through:

1. **Welcome** — a one-screen overview.
2. **API key** — pick the provider you got a key for in Step 2, paste the key. It gets encrypted to your Windows user account and saved at `%APPDATA%\PRACSIM\ModelerAI\keys.dat`. The plain-text key is wiped from memory immediately after.
3. **Cost cap** — optional but recommended for first-time peace of mind. A "soft warning at $X / hard stop at $Y" guard. Start with `warn=$2, stop=$5` if you're nervous.
4. **Model pick** — defaults to Anthropic's Claude Haiku 4.5 (the cheapest at $1/$5 per Mtok). You can pick any model from any provider you've configured a key for. Change later anytime with `/model <id>`.
5. **Done** — Finish, and the chat input becomes active.

You can also skip the whole wizard and configure everything from the Settings tab's **Setup** sub-panel (Settings → Setup). Same effect, no time pressure.

### Step 8 — Send a test message

In the chat input at the bottom, type:

```
Hi, can you see my model?
```

Press Enter or click Send. You'll see "Thinking..." for a few seconds while it talks to the AI provider, then a reply summarizing whatever's in your FlexSim model. After each reply, the header shows token usage and approximate cost (typically tenths of a cent for a one-line question).

**Congratulations.** You're chatting with your FlexSim model.

---

## Optional: use ModelerAI from your phone

ModelerAI ships a local-network remote viewer. In the chat input on your desktop, type:

```
/remote on
```

A QR code appears with a URL like `http://192.168.x.x:12350/#token=...`. Scan it with your phone's camera (your phone needs to be on the same Wi-Fi as the desktop). The same chat surface opens on the phone — send prompts, see streamed replies, approve tool calls. Desktop and phone stay in sync.

Type `/remote off` to stop the LAN server when you're done, or `/remote diag` if you want to see what's connected and watch traffic for troubleshooting.

The viewer uses plain HTTP on the LAN — fine on a home or office Wi-Fi; not appropriate for hostile or guest networks. Don't expose it to the public internet.

---

## Common slash commands

Type `/` in the chat input to see autocomplete. Highlights:

| Command | What it does |
|---|---|
| `/model <id>` | Switch active model. Shortcuts: `haiku`, `sonnet`, `opus`, or any provider's full id (e.g. `gpt-4o`, `gemini-2.5-pro`, `grok-3`). |
| `/effort high` | Turn on extended thinking budget for models that support it (Sonnet, Opus, o1, o3, Gemini 2.5). Smarter but slower and more expensive. |
| `/clear` | Wipe the current conversation and start fresh. |
| `/cost` | Show token usage and cumulative cost for this session. |
| `/export` | Save the current conversation as a Markdown file under `%APPDATA%\PRACSIM\ModelerAI\exports\`. |
| `/tools` | List every tool the agent has, by category (read-only vs mutating). |
| `/rules` | Open your durable user-rules file — instructions the AI re-reads on every message you send. Good for "always use these naming conventions" type things. |
| `/remote on/off/status/diag/trace` | Multi-device remote viewer controls. |
| `/help` | List every command with short descriptions. |

---

## Optional: include the FlexSim user manual in the knowledge base

`setup.bat` builds the AI's knowledge base from FlexSim's **API reference + command reference** only. Those are programmatic reference materials — they list every class, every method, every command. The AI does a good job of answering FlexSim questions with just those.

FlexSim also ships a separate **written user manual** under `<FlexSim install>\manual\` — best-practices guides, walkthroughs of the 3D objects, modeling tips, the dashboard documentation. That content is FlexSim's intellectual property, and to stay clear of FlexSim's EULA we don't redistribute it through this repo.

If you want it indexed for the AI to search, you can opt in **locally, against your own FlexSim install**. The content never leaves your machine. From PowerShell in the `ModelerAI` folder:

```powershell
node tools/ingest-manual.js
node tools/build-kb.js
```

The first command walks `<FlexSim>\manual\**\*.html`, strips the HTML chrome, converts each page to markdown, and writes the results to `KNOWLEDGE\topics\manual\`. The second command rebuilds the search index to include the new topics. Adds ~600 markdown topics (~5 MB) under the gitignored `topics\manual\` folder. Takes a minute or two.

Re-run after a FlexSim update if you want the manual content refreshed.

---

## Where ModelerAI saves things on disk

All ModelerAI files live under `%APPDATA%\PRACSIM\ModelerAI\` (which in PowerShell expands to `C:\Users\<you>\AppData\Roaming\PRACSIM\ModelerAI\`):

| File | Purpose |
|---|---|
| `keys.dat` | Your encrypted API keys. Bound to your Windows user — another Windows account on the same PC cannot read them. |
| `settings.json` | Your preferences (active model, effort, theme, cost cap, etc.). Plain JSON if you ever want to inspect. |
| `sessions/` | Saved conversation history, one file per session anchor. |
| `bridge.log` | Optional debug log of every message between the chat UI and the DLL. Off unless you set the environment variable `MODELER_AI_BRIDGE_LOG=1` before launching FlexSim. |
| `diag.log` | Always-on diagnostic log for the remote viewer subsystem. |
| `exports/` | Where `/export` writes Markdown copies of conversations. |

Nothing ever leaves your computer except your messages to the AI provider you configured. ModelerAI talks directly to the provider using your key; PracSim has no server in the data path. No telemetry, no tracking, no "phone home."

---

## Development loop (if you start editing things)

Once everything works, this is the rhythm for changes:

| What you change | What to run | Reload how |
|---|---|---|
| C++ code under `ModelerAiDLL/` | Press Ctrl + Shift + B in Visual Studio | **Close FlexSim entirely**, then reopen (Windows pins DLLs to a running process — reloading the model alone isn't enough) |
| HTML / JS / CSS under `webview/` | `node tools/embed.js` in PowerShell | Reload the model in FlexSim (Ctrl + R or close/reopen the panel) |
| FlexScript inside `ModelerAI.fsx` | (just save the file) | Reload the model |
| Knowledge-base topic added or edited under `KNOWLEDGE/topics/` | `node tools/build-kb.js` | No reload needed — the agent reads the index lazily on the next `search_kb` call |

---

## Working on the code

If you want to extend ModelerAI — whether by hand or with an AI assistant helping you — start with [`AGENTS.md`](AGENTS.md). It documents the build pipeline (especially the non-obvious `webview/index.html` → `tools/embed.js` → `ModelerAI.fsx` flow), where each kind of thing lives in the source tree, recipes for the common extension tasks (add a curated command, add a slash command, add a KB topic, add a provider, modify the viewer), non-obvious constraints that have bit prior contributors, and where to find diagnostic surfaces when things misbehave.

For commit-message conventions, code style, and how to file issues / PRs, see [`CONTRIBUTING.md`](CONTRIBUTING.md).

---

## Questions, suggestions, or just curious?

Email Josh at **<josh@pracsimsolutions.com>** — happy to hear from you. Whether you want to share a use case, suggest a feature, report something weird, or just chat about whether AI inside FlexSim is a good idea, the inbox is open.

More about the company that built this: [pracsimsolutions.com](https://pracsimsolutions.com).

---

## Contributing

This is MIT-licensed. Fork it, modify it, ship it. See [`CONTRIBUTING.md`](CONTRIBUTING.md) if you want to send fixes or features back upstream.

---

## License

[MIT](LICENSE.txt). Copyright (c) 2026 Practical Simulation Solutions. Provided "as is," no warranty.

AI output may be wrong. **Review what the AI suggests before applying it to important models**, and verify your AI provider's terms of service before sending anything sensitive through it.
