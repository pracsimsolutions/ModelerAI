// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "agent/slash.h"
#include "agent/agent.h"
#include "agent/message.h"
#include "agent/session_store.h"
#include "agent/slash_remote.h"
#include "agent/user_rules.h"
#include "bootstrap.h"
#include "bridge/bridge.h"
#include "bridge/log.h"
#include "paths.h"
#include "provider/provider.h"
#include "settings/effective_settings.h"
#include "third_party/json.h"
#include "tools/tool_base.h"
#include "tools/tool_registry.h"

#include <windows.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>

namespace ModelerAi::agent {

namespace {

// Cost is now computed per-turn inside the Anthropic adapter using the
// active model's pricing (see ModelInfo.*_usd_per_mtok), then accumulated
// into SessionStats.cost_usd by the agent. /cost reads it directly.

std::string makeEnvelope(const char* type, const std::string& turnId, nlohmann::json payload)
{
    nlohmann::json env;
    env["t"]  = type;
    env["id"] = turnId;
    env["p"]  = std::move(payload);
    return env.dump();
}

// Push a synthetic assistant reply: one chunk + done. Used for every slash
// command output so the viewer renders it like a regular reply.
void pushAssistantReply(const std::string& turnId, const std::string& text)
{
    nlohmann::json p1;
    p1["turn_id"] = turnId;
    p1["text"]    = text;
    bridge::pushEvent(makeEnvelope("assistant_chunk", turnId, std::move(p1)));

    nlohmann::json p2;
    p2["turn_id"]    = turnId;
    p2["message_id"] = "";  // synthetic — no provider message id
    bridge::pushEvent(makeEnvelope("assistant_done", turnId, std::move(p2)));
}

// Split "/cmd arg arg" into command (lowercase, no slash) + remainder.
void parseSlash(std::string_view text, std::string& cmd, std::string& rest)
{
    cmd.clear();
    rest.clear();
    std::string s(text);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' ||
                          s.back() == '\r' || s.back() == '\n')) {
        s.pop_back();
    }
    if (s.empty() || s.front() != '/') return;
    s.erase(s.begin());  // drop leading '/'
    auto sp = s.find_first_of(" \t");
    if (sp == std::string::npos) {
        cmd = s;
    } else {
        cmd = s.substr(0, sp);
        rest = s.substr(sp + 1);
        while (!rest.empty() && (rest.front() == ' ' || rest.front() == '\t')) {
            rest.erase(rest.begin());
        }
    }
    for (auto& c : cmd) c = (char)std::tolower((unsigned char)c);
}

// Format a long long with thousands separators (e.g. 1234567 → "1,234,567").
std::string fmtNum(long long n)
{
    std::string s = std::to_string(n);
    // Insert commas every three digits from the right.
    int insertPos = (int)s.size() - 3;
    while (insertPos > 0) {
        s.insert(insertPos, ",");
        insertPos -= 3;
    }
    return s;
}

std::string fmtUsd(double v)
{
    std::ostringstream os;
    os << "$" << std::fixed << std::setprecision(4) << v;
    return os.str();
}

std::string timestampForFilename()
{
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    std::tm local;
    localtime_s(&local, &t);
    std::ostringstream os;
    os << std::put_time(&local, "%Y%m%d-%H%M%S");
    return os.str();
}

std::string roleLabel(Role r)
{
    switch (r) {
        case Role::User:      return "user";
        case Role::Assistant: return "assistant";
        case Role::Tool:      return "tool";
        case Role::System:    return "system";
    }
    return "?";
}

// Try to pretty-print a JSON string. Fall back to the raw input on parse
// failure so we always produce something useful in the export.
std::string prettyJsonForExport(const std::string& raw)
{
    if (raw.empty()) return "(empty)";
    try {
        auto j = nlohmann::json::parse(raw);
        return j.dump(2);
    } catch (...) {
        return raw;
    }
}

bool writeConversationMarkdown(const Conversation& conv,
                               const std::string& outPath)
{
    std::ofstream f(outPath, std::ios::binary);
    if (!f) return false;
    f << "# ModelerAI conversation export\n\n";
    f << "Exported: " << timestampForFilename() << "\n";
    f << "Messages: " << conv.messages.size() << "\n\n";
    f << "---\n\n";
    for (const auto& m : conv.messages) {
        // Header. Bracket the role so it's grep-able and shows the
        // tool_call_id correlation when present (assistant call ↔ tool result).
        f << "## [" << roleLabel(m.role) << "]";
        if (m.role == Role::Tool && m.tool_call_id.has_value()) {
            f << " (response to " << *m.tool_call_id << ")";
        }
        f << "\n\n";
        if (!m.text.empty()) {
            f << m.text << "\n\n";
        }
        // Tool calls — render every call regardless of role. The same
        // ToolCallRecord appears once on the assistant message (args, no
        // result) and again on the tool message (with result), so the
        // export shows the full request/response pair.
        for (const auto& c : m.tool_calls) {
            f << "### tool: " << c.name;
            if (!c.call_id.empty()) f << "  `" << c.call_id << "`";
            f << "\n\n";
            if (!c.args_json.empty()) {
                f << "**args**\n\n```json\n" << prettyJsonForExport(c.args_json) << "\n```\n\n";
            }
            if (!c.result_json.empty()) {
                f << "**result** " << (c.ok ? "ok" : "FAILED") << "\n\n";
                f << "```json\n" << prettyJsonForExport(c.result_json) << "\n```\n\n";
                if (!c.ok && !c.error_message.empty()) {
                    f << "**error**: " << c.error_message << "\n\n";
                }
            }
        }
    }
    return f.good();
}

// ===== individual command handlers =====

void cmdHelp(const std::string& turnId, const std::string& topic)
{
    if (!topic.empty()) {
        // Find the command (matches primary name or alias).
        const CommandInfo* found = nullptr;
        for (const auto& c : commandCatalog()) {
            if (c.name == topic) { found = &c; break; }
            for (const auto& a : c.aliases) if (a == topic) { found = &c; break; }
            if (found) break;
        }
        if (!found) {
            pushAssistantReply(turnId,
                "No such command: /" + topic +
                "\nType /help (with no argument) to see the full list.");
            return;
        }
        std::ostringstream os;
        os << "/" << found->name << " - " << found->description << "\n\n";
        os << found->details << "\n\n";
        os << "Usage: " << found->usage << "\n";
        if (!found->aliases.empty()) {
            os << "Aliases: ";
            for (size_t i = 0; i < found->aliases.size(); ++i) {
                if (i) os << ", ";
                os << "/" << found->aliases[i];
            }
            os << "\n";
        }
        if (found->has_args && !found->args.choices.empty()) {
            os << "\nChoices for <" << found->args.placeholder << ">:\n";
            for (const auto& ch : found->args.choices) {
                os << "  " << ch.label << " - " << ch.description << "\n";
            }
        }
        pushAssistantReply(turnId, os.str());
        return;
    }

    // No topic — list all commands.
    std::ostringstream os;
    os << "Available commands (type /help <command> for details):\n\n";
    for (const auto& c : commandCatalog()) {
        os << "  /" << c.name;
        if (c.has_args && !c.args.placeholder.empty()) {
            os << " <" << c.args.placeholder << ">";
        }
        // Pad to a column for alignment.
        std::string head = "/" + c.name;
        if (c.has_args && !c.args.placeholder.empty()) {
            head += " <" + c.args.placeholder + ">";
        }
        int pad = 22 - (int)head.size();
        if (pad < 2) pad = 2;
        os << std::string(pad, ' ') << c.description << "\n";
    }
    pushAssistantReply(turnId, os.str());
}

void cmdClear(Agent* agent, const std::string& turnId)
{
    int n = agent ? agent->clearConversation() : 0;
    bool deleted = false;
    if (agent) {
        agent->resetStats();
        // Also nuke the on-disk session so the next viewer_ready doesn't
        // re-hydrate the conversation we just cleared.
        std::string key = agent->sessionKey();
        if (!key.empty()) deleted = deleteSession(key);
    }
    std::string msg = "Conversation cleared (" + std::to_string(n) + " messages). "
                      "Token usage counters reset.";
    if (deleted) msg += " On-disk session file removed.";
    pushAssistantReply(turnId, msg);

    if (agent) {
        // Reset Usage UI.
        nlohmann::json p;
        p["key"] = "session_stats";
        nlohmann::json zero;
        zero["input_tokens"]                = 0;
        zero["output_tokens"]               = 0;
        zero["cache_read_input_tokens"]     = 0;
        zero["cache_creation_input_tokens"] = 0;
        zero["turns"]                       = 0;
        zero["cost_usd"]                    = 0.0;
        p["value"] = std::move(zero);
        bridge::pushEvent(makeEnvelope("state_update", turnId, std::move(p)));
    }
}

void cmdCost(Agent* agent, const std::string& turnId)
{
    if (!agent) { pushAssistantReply(turnId, "Agent not available."); return; }
    SessionStats s = agent->stats();
    std::ostringstream os;
    os << "Session usage:\n";
    os << "  Input tokens:       " << fmtNum(s.input_tokens) << "\n";
    os << "  Output tokens:      " << fmtNum(s.output_tokens) << "\n";
    os << "  Cache read tokens:  " << fmtNum(s.cache_read_input_tokens)
       << "  (10% of input rate)\n";
    os << "  Cache write tokens: " << fmtNum(s.cache_creation_input_tokens)
       << "  (25% surcharge)\n";
    os << "  Turns:              " << s.turns << "\n";
    os << "  Estimated cost:     " << fmtUsd(s.cost_usd) << "\n";
    os << "\nCost is summed per-turn at the rate of whichever model answered "
          "that turn, so switching models mid-session stays accurate.";
    pushAssistantReply(turnId, os.str());
}

void cmdModel(Agent* agent, const std::string& turnId, const std::string& arg)
{
    if (!agent || !agent->provider()) {
        pushAssistantReply(turnId, "No provider configured.");
        return;
    }
    auto* prov = agent->provider();

    if (!arg.empty()) {
        // Resolve user-friendly aliases like "haiku" / "sonnet" / "opus"
        // to canonical IDs. Otherwise treat arg as a full model ID.
        std::string target = arg;
        if (target == "haiku")  target = "claude-haiku-4-5";
        if (target == "sonnet") target = "claude-sonnet-4-6";
        if (target == "opus")   target = "claude-opus-4-8";

        if (!prov->set_active_model(target)) {
            std::ostringstream err;
            err << "Unknown model: " << arg << "\n\nAvailable:\n";
            for (const auto& m : prov->models()) {
                err << "  " << m.display_name << "  (" << m.id << ")\n";
            }
            pushAssistantReply(turnId, err.str());
            return;
        }
    }

    auto models = prov->models();
    std::string activeId = prov->active_model_id();
    const provider::ModelInfo* active = nullptr;
    for (const auto& m : models) if (m.id == activeId) { active = &m; break; }

    std::ostringstream os;
    os << "Active provider: " << prov->display_name() << "\n";
    if (active) {
        os << "Active model:    " << active->display_name
           << "  (" << active->id << ")\n";
        os << "Capabilities:    "
           << (active->supports_tools ? "tools " : "")
           << (active->supports_vision ? "vision " : "")
           << (active->supports_thinking ? "thinking " : "")
           << "ctx=" << active->context_tokens << "\n";
        os << "Pricing:         $"
           << active->input_usd_per_mtok  << "/Mtok in, $"
           << active->output_usd_per_mtok << "/Mtok out\n";
    }
    if (!arg.empty()) {
        os << "\nSwitched - subsequent turns use this model.";
    } else if (models.size() > 1) {
        os << "\nAvailable models (use /model <id> to switch):\n";
        for (const auto& m : models) {
            os << (m.id == activeId ? "  * " : "    ")
               << m.display_name << "  (" << m.id << ")\n";
        }
    }
    pushAssistantReply(turnId, os.str());
}

void cmdEffort(Agent* agent, const std::string& turnId, const std::string& arg)
{
    if (!agent || !agent->provider()) {
        pushAssistantReply(turnId, "No provider configured.");
        return;
    }
    auto* prov = agent->provider();

    if (arg.empty()) {
        const char* current = "off";
        switch (prov->effort()) {
            case provider::EffortLevel::Off:    current = "off";    break;
            case provider::EffortLevel::Low:    current = "low";    break;
            case provider::EffortLevel::Medium: current = "medium"; break;
            case provider::EffortLevel::High:   current = "high";   break;
        }
        std::ostringstream os;
        os << "Current effort: " << current << "\n";
        os << "Use /effort off|low|medium|high to change.\n";
        os << "Higher effort = more silent reasoning per turn = better answers on "
              "hard questions, at extra output-token cost. Only models with "
              "thinking support honor this setting (currently all three "
              "Anthropic models do).";
        pushAssistantReply(turnId, os.str());
        return;
    }

    provider::EffortLevel level;
    if      (arg == "off"    || arg == "none") level = provider::EffortLevel::Off;
    else if (arg == "low"    || arg == "lo")   level = provider::EffortLevel::Low;
    else if (arg == "medium" || arg == "med")  level = provider::EffortLevel::Medium;
    else if (arg == "high"   || arg == "hi")   level = provider::EffortLevel::High;
    else {
        pushAssistantReply(turnId,
            "Unknown effort level: " + arg +
            "\nValid choices: off, low, medium, high.");
        return;
    }
    prov->set_effort(level);

    const char* result = "off";
    switch (prov->effort()) {
        case provider::EffortLevel::Off:    result = "off";    break;
        case provider::EffortLevel::Low:    result = "low";    break;
        case provider::EffortLevel::Medium: result = "medium"; break;
        case provider::EffortLevel::High:   result = "high";   break;
    }
    if (std::string(result) != arg && arg != "none" && arg != "lo" && arg != "med" && arg != "hi") {
        // set_effort silently no-ops if the model doesn't support thinking.
        pushAssistantReply(turnId,
            "Effort change ignored - active model doesn't support extended thinking. "
            "Current effort: " + std::string(result));
    } else {
        pushAssistantReply(turnId, "Effort set to " + std::string(result) + ".");
    }
}

void cmdExport(Agent* agent, const std::string& turnId)
{
    if (!agent) { pushAssistantReply(turnId, "Agent not available."); return; }

    Conversation conv = agent->conversationCopy();
    if (conv.messages.empty()) {
        pushAssistantReply(turnId, "Nothing to export - conversation is empty.");
        return;
    }

    // Resolve target directory: setting wins; empty falls back to the
    // %APPDATA% default. Trailing slash optional either way.
    std::string exportDir;
    auto eff = settings::resolveEffectiveSettings();
    if (!eff.export_dir.empty()) {
        exportDir = eff.export_dir;
        if (exportDir.back() != '\\' && exportDir.back() != '/') exportDir += '\\';
    } else {
        std::string dir = paths::appDataDir();
        if (dir.empty()) {
            pushAssistantReply(turnId, "Couldn't resolve %APPDATA% to write the export. "
                                       "Set Settings -> General -> Export folder to a writable path.");
            return;
        }
        exportDir = dir + "exports\\";
    }
    CreateDirectoryA(exportDir.c_str(), nullptr);  // idempotent
    std::string path = exportDir + "conversation-" + timestampForFilename() + ".md";

    if (!writeConversationMarkdown(conv, path)) {
        pushAssistantReply(turnId,
            "Failed to write export file: " + path +
            "\n\nIs the folder writable? You can change the target in "
            "Settings -> General -> Export folder.");
        return;
    }
    pushAssistantReply(turnId,
        "Exported " + std::to_string(conv.messages.size())
        + " messages to:\n  " + path);
}

} // namespace

namespace {

CommandInfo makeCmd(std::string name,
                    std::vector<std::string> aliases,
                    std::string description,
                    std::string details,
                    std::string usage,
                    ArgSpec args = {},
                    bool hasArgs = false)
{
    CommandInfo c;
    c.name        = std::move(name);
    c.aliases     = std::move(aliases);
    c.description = std::move(description);
    c.details     = std::move(details);
    c.usage       = std::move(usage);
    c.args        = std::move(args);
    c.has_args    = hasArgs;
    return c;
}

ArgSpec helpArgSpec()
{
    ArgSpec s;
    s.placeholder = "command";
    s.choices = {
        { "help",   "/help",   "Show available commands" },
        { "clear",  "/clear",  "Wipe conversation history" },
        { "cost",   "/cost",   "Show token usage and cost" },
        { "model",  "/model",  "Show or switch the active model" },
        { "effort", "/effort", "Set extended-thinking budget" },
        { "export", "/export", "Save conversation as markdown" },
        { "rules",  "/rules",  "Open your persistent rules file" },
    };
    return s;
}

ArgSpec modelArgSpec()
{
    // Choices reflect the Anthropic catalog hardcoded in anthropic.cpp.
    // When multi-provider lands (v0.2 OpenAI-compatible adapter), this
    // spec should be built dynamically from the bridge / agent instead.
    ArgSpec s;
    s.placeholder = "model";
    s.choices = {
        { "claude-haiku-4-5",  "Haiku 4.5",  "Cheapest + fastest ($1/$5 per Mtok). Default." },
        { "claude-sonnet-4-6", "Sonnet 4.6", "Balanced ($3/$15 per Mtok)" },
        { "claude-opus-4-8",   "Opus 4.8",   "Smartest ($5/$25 per Mtok)" },
    };
    return s;
}

ArgSpec effortArgSpec()
{
    ArgSpec s;
    s.placeholder = "level";
    s.choices = {
        { "off",    "off",    "Disable extended thinking (default, cheapest)" },
        { "low",    "low",    "~2k thinking tokens — quick reasoning" },
        { "medium", "medium", "~8k thinking tokens — standard reasoning" },
        { "high",   "high",   "~16k thinking tokens — heavy reasoning" },
    };
    return s;
}

ArgSpec remoteArgSpec()
{
    ArgSpec s;
    s.placeholder = "subcommand";
    s.choices = {
        { "on",     "on",     "Start the server (default port 12350)." },
        { "off",    "off",    "Stop the server, kick clients." },
        { "status", "status", "Print current state + QR." },
        { "rotate", "rotate", "Regenerate the token (kicks current clients)." },
        { "diag",   "diag",   "Dump live state (server, subscribers, queues)." },
        { "trace",  "trace",  "Toggle verbose diagnostic logging (on|off)." },
    };
    return s;
}

} // namespace

const std::vector<CommandInfo>& commandCatalog()
{
    // Single source of truth for slash commands. The viewer renders these
    // as autocomplete entries; the dispatch switch below must handle every
    // name + alias listed here.
    static const std::vector<CommandInfo> kCatalog = {
        makeCmd("help",
                {"?"},
                "Show available commands",
                "Lists every slash command. Pass a command name to see "
                "its details: try /help clear or /help cost.",
                "/help [command]",
                helpArgSpec(),
                /*hasArgs*/ true),

        makeCmd("clear",
                {"new", "reset"},
                "Wipe conversation history and reset token counters",
                "Drops the entire conversation from memory and zeroes the "
                "session token counters. The system prompt, user rules, "
                "and your selected model stay put. Cost: nothing - no API "
                "call is made.",
                "/clear"),

        makeCmd("cost",
                {"tokens", "usage"},
                "Show running token usage and dollar estimate for this session",
                "Displays input / output / cache-read / cache-write token "
                "totals for the current session plus an estimated USD cost. "
                "Cost is summed per-turn at the rate of whichever model "
                "answered that turn, so switching models mid-session stays "
                "accurate. The figure is a client-side estimate, not an "
                "authoritative bill - check console.anthropic.com for the truth.",
                "/cost"),

        makeCmd("model",
                {},
                "Show or switch the active model",
                "Without an argument, reports the currently active model. "
                "With a model id, switches to it for subsequent turns. "
                "Three Anthropic models are available: Haiku 4.5 (cheapest), "
                "Sonnet 4.6 (default), Opus 4.7 (smartest). OpenAI / Gemini "
                "land in v0.2 alongside the generic OpenAI-compatible adapter.",
                "/model [haiku|sonnet|opus|<full-id>]",
                modelArgSpec(),
                /*hasArgs*/ true),

        makeCmd("effort",
                {},
                "Set extended-thinking budget",
                "Controls how much hidden reasoning the model does before "
                "replying. Off (default) is cheapest. Higher levels burn "
                "extra output tokens for deeper reasoning - useful for "
                "tricky FlexSim questions. No-op on models without "
                "thinking support.",
                "/effort off|low|medium|high",
                effortArgSpec(),
                /*hasArgs*/ true),

        makeCmd("export",
                {"save"},
                "Save the conversation as markdown",
                "Writes the full conversation (every user + assistant turn) "
                "to a timestamped markdown file under "
                "%APPDATA%\\PRACSIM\\ModelerAI\\exports\\. Useful for "
                "sharing a session or archiving before /clear.",
                "/export"),

        makeCmd("tools",
                {},
                "List the tools the model can currently call",
                "Shows every registered tool the LLM can choose to invoke "
                "this turn. Each entry includes the tool's category "
                "(read-only, mutating, etc.) and a short description. "
                "FlexSim-aware tools are registered as stubs ship-side; "
                "their bodies fill in incrementally.",
                "/tools"),

        makeCmd("rules",
                {"train", "teach"},
                "Open your persistent rules file (durable training)",
                "Opens %APPDATA%\\PRACSIM\\ModelerAI\\rules.md in your "
                "default editor. Anything you write here is loaded into "
                "EVERY future conversation as authoritative training — the "
                "AI treats your rules as overrides to its defaults. Use it "
                "for things you keep having to re-tell: naming conventions, "
                "default distributions, project-specific units, build "
                "preferences. The rules persist across model restarts and "
                "across all your sessions.",
                "/rules"),

        makeCmd("remote",
                {},
                "Manage the LAN remote-viewer server (phone access).",
                "Start/stop/inspect the HTTP server that lets phones on the "
                "same LAN connect to this chat. Tokens regenerate on every "
                "start; lifecycle ends with the panel.",
                "/remote on | off | status | rotate | diag | trace",
                remoteArgSpec(),
                /*hasArgs*/ true),
    };
    return kCatalog;
}

bool isSlashCommand(std::string_view text)
{
    for (char c : text) {
        if (c == ' ' || c == '\t') continue;
        return c == '/';
    }
    return false;
}

bool dispatchSlash(Agent* agent,
                   const std::string& turnId,
                   std::string_view text)
{
    std::string cmd, rest;
    parseSlash(text, cmd, rest);

    bridge::consolePrint(("[ModelerAI] slash command: /" + cmd + "\n"));

    if (cmd == "help" || cmd == "?") {
        // /help with optional argument like "/help clear"
        std::string topic = rest;
        // Strip a leading slash from the topic if the user typed "/help /clear"
        while (!topic.empty() && (topic.front() == ' ' || topic.front() == '/')) {
            topic.erase(topic.begin());
        }
        // Lowercase for lookup.
        for (auto& ch : topic) ch = (char)std::tolower((unsigned char)ch);
        cmdHelp(turnId, topic); return true;
    }
    if (cmd == "clear" || cmd == "new" || cmd == "reset") {
        cmdClear(agent, turnId); return true;
    }
    if (cmd == "cost" || cmd == "tokens" || cmd == "usage") {
        cmdCost(agent, turnId); return true;
    }
    if (cmd == "model") {
        // Strip any leading slash from the arg (handles "/model /claude-...").
        std::string arg = rest;
        while (!arg.empty() && (arg.front() == ' ' || arg.front() == '/')) {
            arg.erase(arg.begin());
        }
        for (auto& c : arg) c = (char)std::tolower((unsigned char)c);
        cmdModel(agent, turnId, arg); return true;
    }
    if (cmd == "effort") {
        std::string arg = rest;
        while (!arg.empty() && arg.front() == ' ') arg.erase(arg.begin());
        for (auto& c : arg) c = (char)std::tolower((unsigned char)c);
        cmdEffort(agent, turnId, arg); return true;
    }
    if (cmd == "export" || cmd == "save") {
        cmdExport(agent, turnId); return true;
    }
    if (cmd == "tools") {
        auto* reg = bootstrap::toolRegistry();
        std::ostringstream os;
        if (!reg || reg->size() == 0) {
            os << "No tools are registered.";
        } else {
            os << "Registered tools (" << reg->size() << "):\n";
            for (const auto& def : reg->toolDefs()) {
                os << "  " << def.name << " — " << def.description << "\n";
            }
            os << "\nThe LLM picks which tool to call (if any) based on your question. "
                  "Pure framework — actual FlexSim-aware tools are authored by the user "
                  "incrementally.";
        }
        pushAssistantReply(turnId, os.str());
        return true;
    }
    if (cmd == "remote") {
        return dispatchRemote(agent, turnId, rest);
    }
    if (cmd == "rules" || cmd == "train" || cmd == "teach") {
        // Open the user's persistent rules file in their default editor.
        // The first loadUserRules() call seeds a template if the file
        // doesn't exist yet, so by the time we ShellExecute it, there's
        // always something to edit.
        (void)loadUserRules();
        std::string path = userRulesPath();
        std::ostringstream os;
        if (path.empty()) {
            os << "Could not resolve the rules file path (APPDATA lookup failed).";
            pushAssistantReply(turnId, os.str());
            return true;
        }
        // ShellExecuteA with "open" verb hands the file to whatever the
        // user has registered for .md files (VS Code, Notepad, etc.).
        HINSTANCE rc = ShellExecuteA(nullptr, "open", path.c_str(),
                                     nullptr, nullptr, SW_SHOWNORMAL);
        if ((INT_PTR)rc <= 32) {
            os << "Couldn't open " << path << " automatically. "
               << "Open it manually to edit your persistent rules.";
        } else {
            os << "Opened your rules file: " << path << "\n\n"
               << "Anything you write there loads into EVERY future "
               << "conversation as authoritative training. Common uses:\n"
               << "  - Naming conventions (\"name all Sources SRC_*\")\n"
               << "  - Default values (\"default arrivals are exponential(60)\")\n"
               << "  - Build preferences (\"always 2m clearance between objects\")\n"
               << "  - Domain context (\"this is a warehouse model — Operators are pickers\")\n"
               << "  - Discovery directives (\"before any multi-step build, read flexsim-picklist-library first\")\n\n"
               << "Changes take effect on the next message you send (no restart needed).";
        }
        pushAssistantReply(turnId, os.str());
        return true;
    }
    // Unknown
    pushAssistantReply(turnId,
        "Unknown command: /" + cmd + "\nType /help to see what's available.");
    return false;
}

} // namespace ModelerAi::agent
