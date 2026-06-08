# ModelerAiDLL — C++ DLL for the ModelerAI module

> Copyright (c) 2026 Practical Simulation Solutions. MIT License — see `LICENSE.txt` at the repo root.

The compiled DLL owns almost all business logic for ModelerAI: provider clients (Anthropic, OpenAI, Google Gemini, xAI Grok, Mistral, plus an OpenAI-compatible adapter for everything else), the agent loop, tool implementations, knowledge-base indexing + retrieval, the multi-device remote viewer's HTTP server, DPAPI-encrypted BYOK key storage, and the JSON bridge that talks to the HTML viewer running inside FlexSim's CEF panel.

The HTML viewer (`../webview/`) is a dumb renderer with no knowledge of providers, prompts, or tool definitions — it sends user intents over the bridge and renders chunks it receives back. The split keeps the bridge protocol auditable and the JS surface small; it's not an IP wall. See [`../AGENTS.md`](../AGENTS.md) for the build pipeline and the full source-tree map.

## Layout

```
ModelerAiDLL/
├── ModelerAi.sln               Visual Studio solution
├── module.vcxproj              C++ project. Add new source files to BOTH this
│                                file's <ItemGroup>s AND module.vcxproj.filters
│                                (Solution Explorer → Add → Existing Item does
│                                both for you).
├── module.cpp / .h             FlexSim DllMain + exports
├── abi.cpp / .h                ModelerAi_bridgeSend / ModelerAi_bridgePoll
│                                — the two FlexScript-callable bridge entries.
├── bootstrap.cpp / .h          Init / teardown wiring; builds the agent +
│                                provider list at DLL load.
├── targetver.h                 Win32 SDK version pin.
├── copyheaders.bat             Pre-build event: copies FlexSim SDK headers
│                                from the surrounding install into
│                                ./flexsimcontent/.
│
├── bridge/                     JSON-envelope protocol between DLL and viewer:
│                                handleEnvelope, pushEvent / pushEventTo
│                                (per-subscriber), the thread-safe diag log.
├── agent/                      Agent loop, slash commands (slash.cpp + the
│                                slash_*.cpp modules), prompt assembly, user
│                                rules, session storage + restore.
├── provider/                   Provider abstraction (IProvider) + adapters:
│                                Anthropic, Gemini, builtin_chat (OpenAI/xAI/
│                                Mistral via shared OpenAI-Chat-Completions
│                                code), openai_compatible (custom providers).
├── tools/                      Tool implementations the agent can call:
│                                read/ (ask_user_question, kb_tools, stubs)
│                                write/ (run_script, modelerai_call)
├── commands/                   The modelerai_* curated command library
│                                (create_object, set_property, etc.) plus
│                                the FlexScript-aware identifier validator.
├── kb/                         Markdown topic loader + BM25 search.
├── keystore/                   DPAPI-encrypted BYOK key storage; bound to
│                                the Windows user account.
├── remote/                     LAN remote viewer (cpp-httplib HTTP server,
│                                LAN interface detection, QR payload).
├── settings/                   Effective-settings resolution (per-project
│                                overrides on top of global defaults).
├── mainthread/                 Worker → FlexSim-main-thread task queue, for
│                                SDK calls from the agent's worker threads.
├── tree/                       Active-model condense pass (used by the
│                                agent's prompt assembly).
└── third_party/                Vendored single-header libs:
    ├── thirdparty/nlohmann/    nlohmann/json (via the defensive wrapper
    │                            at third_party/json.h)
    └── cpp-httplib/            cpp-httplib v0.15.3 for the remote server.
```

## Where to start

1. Open `ModelerAi.sln` in Visual Studio 2022 (with the C++ desktop workload installed).
2. The pre-build event runs `copyheaders.bat` to drop FlexSim SDK headers into `./flexsimcontent/`. That requires the repo to be sitting inside a FlexSim install, so first **clone the repo into your FlexSim 2026 install's modules directory** (`C:\Program Files\FlexSim 2026\modules\ModelerAI\` under a default install).
3. Build **x64 / Debug** or **x64 / Release**. Either configuration produces `ModelerAI.dll` at the repo root (Win32 is supported but produces `ModelerAI_x86.dll`).
4. Open a model in FlexSim — the DLL loads automatically on module init. Run `addmodelerai` in the FlexScript Console to open the chat panel.

If the build fails with `LNK1104 cannot open file 'ModelerAI.dll'`, FlexSim is currently running and has the old DLL loaded — close FlexSim entirely and rebuild.

## Adding files to the project

When you start implementing a new subsystem (or split an existing one), add its headers + sources to:
- `module.vcxproj` — two `ItemGroup`s, one for `ClInclude` and one for `ClCompile`.
- `module.vcxproj.filters` — same two ItemGroups, with `<Filter>` entries so the files show up in the correct Visual Studio filter folder.

Solution Explorer → Add → Existing Item does both for you.

## Coding conventions

- **Namespace:** `ModelerAi::<area>` (note the lowercase `i`).
- **C++20**, set via `<LanguageStandard>stdcpp20</LanguageStandard>` in `module.vcxproj`.
- **Per-file copyright header** on every new source file:
  ```cpp
  // Copyright (c) 2026 Practical Simulation Solutions.
  // Licensed under the MIT License — see LICENSE.txt at the repo root.
  ```
- **Multi-byte character set** (matches FlexSim's SDK). No `wchar` / `wstring` at boundaries.
- **WinHTTP** for HTTPS, not libcurl. Matches how the provider streaming clients are written; no extra dependency.
- **nlohmann/json** for JSON, vendored at `third_party/thirdparty/nlohmann/json.hpp`. Always include it via the defensive wrapper at `third_party/json.h` — it `#undef`s the Windows / FlexSim SDK macros (`small`, `o`, `ip`, `IN`, `OUT`, `OPTIONAL`) that otherwise collide with identifiers inside json.hpp.
- **Anonymous namespaces** for TU-local helpers. Functions that need external linkage (callable from `abi.cpp` etc.) go in the public `ModelerAi::<area>` namespace.

For everything else — extension recipes (add a provider, add a slash command, add a tool, add a KB topic, modify the viewer), gotchas (FlexSim main-thread rules, DLL process-pinning, JS IIFE scoping), and diagnostic surfaces — read [`../AGENTS.md`](../AGENTS.md).
