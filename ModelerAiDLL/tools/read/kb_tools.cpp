// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "tools/read/kb_tools.h"

#include "bootstrap.h"
#include "kb/kb_index.h"
#include "kb/kb_loader.h"
#include "third_party/json.h"

#include <algorithm>
#include <cctype>

namespace ModelerAi::tools {

namespace {

// Strip / replace invalid UTF-8 sequences so nlohmann::json::dump() doesn't
// throw json::exception::type_error.316. Some KB topic bodies (especially
// those ingested from FlexSim's Windows-1252 manual XML) carry stray
// high-bytes that aren't valid UTF-8 standalone — they pass through string
// assignment fine but explode at JSON serialization time. We sanitize once
// at the boundary so search results never blow up the whole tool call over
// one bad byte in an excerpt.
//
// Strategy: scan as UTF-8; on any malformed sequence advance one byte and
// emit U+FFFD (replacement char). Valid sequences pass through verbatim.
std::string toValidUtf8(std::string_view s)
{
    std::string out;
    out.reserve(s.size());
    auto byte = [&](std::size_t i) -> unsigned char {
        return static_cast<unsigned char>(s[i]);
    };
    std::size_t i = 0;
    while (i < s.size()) {
        unsigned char b0 = byte(i);
        std::size_t need = 0;
        if      (b0 < 0x80) { out.push_back(static_cast<char>(b0)); ++i; continue; }
        else if ((b0 & 0xE0) == 0xC0) { need = 1; }
        else if ((b0 & 0xF0) == 0xE0) { need = 2; }
        else if ((b0 & 0xF8) == 0xF0) { need = 3; }
        else                          { out.append("\xEF\xBF\xBD"); ++i; continue; }

        if (i + need >= s.size()) { out.append("\xEF\xBF\xBD"); ++i; continue; }
        bool ok = true;
        for (std::size_t k = 1; k <= need; ++k) {
            if ((byte(i + k) & 0xC0) != 0x80) { ok = false; break; }
        }
        if (!ok) { out.append("\xEF\xBF\xBD"); ++i; continue; }
        for (std::size_t k = 0; k <= need; ++k) out.push_back(s[i + k]);
        i += need + 1;
    }
    return out;
}

// Extract a short excerpt from `body` centered on the best match of the
// query's tokens. Returns up to ~maxChars characters trimmed to word
// boundaries with ellipsis on each side as needed. If no query tokens
// match, returns the first maxChars chars.
std::string makeExcerpt(const std::string& body, std::string_view query,
                        std::size_t maxChars = 320)
{
    if (body.empty()) return "";
    // Lowercased copy for case-insensitive match.
    std::string lower; lower.reserve(body.size());
    for (char c : body) lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));

    // Tokenize the query and look for the leftmost match of any token in
    // the body. We don't need the strongest match — just SOMETHING relevant
    // for a snippet. BM25 already decided this topic was a strong hit.
    std::size_t bestPos = std::string::npos;
    std::string cur;
    for (char c : query) {
        char lc = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if ((lc >= 'a' && lc <= 'z') || (lc >= '0' && lc <= '9') || lc == '_') {
            cur.push_back(lc);
        } else if (!cur.empty()) {
            if (cur.size() >= 3) {
                auto p = lower.find(cur);
                if (p != std::string::npos && (bestPos == std::string::npos || p < bestPos)) {
                    bestPos = p;
                }
            }
            cur.clear();
        }
    }
    if (!cur.empty() && cur.size() >= 3) {
        auto p = lower.find(cur);
        if (p != std::string::npos && (bestPos == std::string::npos || p < bestPos)) {
            bestPos = p;
        }
    }

    std::size_t start = 0;
    if (bestPos != std::string::npos && bestPos > maxChars / 3) {
        start = bestPos - (maxChars / 3);
        // Walk back to a word boundary.
        while (start > 0 && body[start] != ' ' && body[start] != '\n') --start;
        if (body[start] == ' ' || body[start] == '\n') ++start;
    }

    std::size_t end = std::min(start + maxChars, body.size());
    // Walk forward to a word boundary.
    while (end < body.size() && body[end] != ' ' && body[end] != '\n') ++end;

    std::string snippet = body.substr(start, end - start);
    // Collapse internal whitespace to single spaces — markdown line breaks
    // waste tokens in a snippet context.
    std::string out;
    out.reserve(snippet.size());
    bool prevSpace = false;
    for (char c : snippet) {
        if (c == '\n' || c == '\r' || c == '\t' || c == ' ') {
            if (!prevSpace && !out.empty()) {
                out.push_back(' ');
                prevSpace = true;
            }
        } else {
            out.push_back(c);
            prevSpace = false;
        }
    }
    // Trim trailing space.
    while (!out.empty() && out.back() == ' ') out.pop_back();
    if (start > 0) out = "…" + out;
    if (end < body.size()) out.push_back('…');
    return out;
}

} // namespace

// ---- search_kb ----------------------------------------------------------

std::string SearchKbTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "query": {
      "type": "string",
      "description": "Free-text query. Matches against topic name, signature, description, tags, and aliases. Symbol-shape queries short-circuit BM25 ('AGV::Trailer', 'setrank'); free-text queries run full BM25 ('conveyor speed', 'how does push/pull work')."
    },
    "limit": {
      "type": "integer",
      "description": "Max results to return. Default 8. Cap 25.",
      "minimum": 1,
      "maximum": 25
    },
    "kind": {
      "type": "string",
      "description": "Restrict to one kind. Optional. 'class' = FlexScript classes from API XML. 'command' = FlexScript commands. 'manual' = ingested manual chapters (best for conceptual questions).",
      "enum": ["class", "command", "manual"]
    },
    "module": {
      "type": "string",
      "description": "Restrict to a single FlexSim module (e.g. 'AGV', 'Conveyor', 'ProcessFlow'). Optional. Classes only — commands have no module."
    },
    "tags": {
      "type": "array",
      "items": { "type": "string" },
      "description": "Restrict to topics that include ALL of these tags (intersection). Useful for scoping a manual search to a chapter — pass ['bestpractices'] or ['modellogic','processflowbasics']. Manual topics carry chapter-path tags automatically. Optional."
    },
    "include_deprecated": {
      "type": "boolean",
      "description": "Include deprecated commands in results. Default false. Turn on only when you need migration info."
    },
    "include_excerpt": {
      "type": "boolean",
      "description": "Include a ~300-char body excerpt centered on the query terms in each result. Default true — set false when you only need to know which topics exist (saves tokens)."
    }
  },
  "required": ["query"]
})";
}

ToolResult SearchKbTool::run(std::string_view args_json)
{
    ToolResult r;
    // Args-parsing has its own catch so we can mislabel-proof the error:
    // anything that throws DEEPER (KB internals, dump(), excerpt sanitize)
    // shouldn't surface as "bad_args_json".
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(args_json);
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "bad_args_json";
        r.error_message = std::string("Couldn't parse args JSON: ") + e.what();
        return r;
    }
    try {
        std::string query = j.value("query", "");
        if (query.empty()) {
            r.ok = false;
            r.error_code = "missing_query";
            r.error_message = "search_kb expects a non-empty 'query' string.";
            return r;
        }
        kb::SearchOptions opts;
        opts.limit             = j.value("limit", 8);
        opts.kindFilter        = j.value("kind", "");
        opts.moduleFilter      = j.value("module", "");
        opts.includeDeprecated = j.value("include_deprecated", false);
        if (j.contains("tags") && j["tags"].is_array()) {
            for (const auto& t : j["tags"]) {
                if (t.is_string()) opts.tagFilters.push_back(t.get<std::string>());
            }
        }
        const bool includeExcerpt = j.value("include_excerpt", true);

        auto* idx = bootstrap::kbIndex();
        if (!idx || !idx->loaded()) {
            r.ok = false;
            r.error_code = "kb_unavailable";
            r.error_message = "KB index not loaded. Run tools/build-kb.js to regenerate KNOWLEDGE/INDEX.json.";
            return r;
        }

        auto hits = idx->search(query, opts);
        const auto& topics = idx->topics();

        nlohmann::json out;
        out["query"]   = query;
        out["count"]   = static_cast<int>(hits.size());
        out["results"] = nlohmann::json::array();
        for (const auto& h : hits) {
            const auto& m = topics[h.topicIdx];
            nlohmann::json hit;
            hit["id"]          = m.id;
            hit["name"]        = m.name;
            hit["kind"]        = m.kind;
            if (!m.module.empty())     hit["module"]     = m.module;
            if (!m.group.empty())      hit["group"]      = m.group;
            if (!m.signature.empty())  hit["signature"]  = m.signature;
            if (!m.description.empty())hit["description"]= m.description;
            if (!m.aliases.empty()) {
                // Cap the alias list at 6 per result — the LLM only needs a
                // hint about why a topic matched, not the entire bag.
                nlohmann::json arr = nlohmann::json::array();
                int n = static_cast<int>(m.aliases.size());
                if (n > 6) n = 6;
                for (int k = 0; k < n; ++k) arr.push_back(m.aliases[k]);
                if (static_cast<int>(m.aliases.size()) > 6) {
                    arr.push_back("+" + std::to_string(m.aliases.size() - 6) + " more");
                }
                hit["aliases"] = std::move(arr);
            }
            if (m.deprecated)              hit["deprecated"] = true;
            if (!m.replacedBy.empty())     hit["replaced_by"] = m.replacedBy;
            // Score is helpful for debugging but not critical; expose a
            // rounded value so the LLM can spot weak matches.
            hit["score"] = std::round(h.score * 100.0) / 100.0;
            // Body excerpt centered on the query terms — turns "found these
            // topics" into "found these topics and here's the relevant
            // sentence" in one tool call. Skip when include_excerpt=false
            // or the topic's kind has no fetchable body (would be empty).
            if (includeExcerpt) {
                std::string kindDir;
                if (m.kind == "class")   kindDir = "classes";
                else if (m.kind == "command") kindDir = "commands";
                else if (m.kind == "manual")  kindDir = "manual";
                if (!kindDir.empty()) {
                    std::string body = kb::loadTopicBody(kindDir, m.id);
                    if (!body.empty()) {
                        std::string ex = makeExcerpt(body, query, 320);
                        if (!ex.empty()) hit["excerpt"] = toValidUtf8(ex);
                    }
                }
            }
            out["results"].push_back(std::move(hit));
        }
        if (hits.empty()) {
            out["note"] = "No matches. Try broader/shorter terms, or include_deprecated=true.";
        }
        r.ok = true;
        r.result_json = out.dump();
        return r;
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "search_failed";
        r.error_message = std::string("search_kb threw: ") + e.what();
        return r;
    }
}

// ---- read_topic ---------------------------------------------------------

std::string ReadTopicTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "id": {
      "type": "string",
      "description": "Topic slug. For classes use lowercase with '__' between module and class (e.g. 'agv__trailer', 'conveyor__straightconveyor'). For commands use the lowercase command name (e.g. 'createcopy')."
    },
    "kind": {
      "type": "string",
      "description": "Optional: 'class' or 'command'. If omitted, tries both — slightly slower.",
      "enum": ["class", "command"]
    }
  },
  "required": ["id"]
})";
}

ToolResult ReadTopicTool::run(std::string_view args_json)
{
    ToolResult r;
    nlohmann::json j;
    try {
        j = nlohmann::json::parse(args_json);
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "bad_args_json";
        r.error_message = std::string("Couldn't parse args JSON: ") + e.what();
        return r;
    }
    try {
        std::string id   = j.value("id", "");
        std::string kind = j.value("kind", "");
        if (id.empty()) {
            r.ok = false;
            r.error_code = "missing_id";
            r.error_message = "read_topic expects an 'id' string.";
            return r;
        }

        std::string body;
        std::string resolvedKind;

        if (!kind.empty()) {
            // Map provider-facing singulars ("class"/"command") to filesystem
            // directories ("classes"/"commands").
            std::string dir;
            if      (kind == "class")   dir = "classes";
            else if (kind == "command") dir = "commands";
            else                        dir = kind; // tolerate exact dir names

            body = kb::loadTopicBody(dir, id);
            resolvedKind = kind;
        } else {
            // Resolution order: classes → commands → manual → root.
            // Slug collisions across these are extremely unlikely (class
            // slugs use PascalCase + '__', commands are all-lowercase,
            // manual ids are lowercase_with_underscores). First hit wins.
            body = kb::loadTopicBody("classes", id);
            if (!body.empty()) resolvedKind = "class";
            else {
                body = kb::loadTopicBody("commands", id);
                if (!body.empty()) resolvedKind = "command";
                else {
                    body = kb::loadTopicBody("manual", id);
                    if (!body.empty()) resolvedKind = "manual";
                    else {
                        // Last resort: hand-written topic at KNOWLEDGE/topics/<id>.md
                        body = kb::loadTopicBody(id);
                        if (!body.empty()) resolvedKind = "pattern";
                    }
                }
            }
        }

        if (body.empty()) {
            r.ok = false;
            r.error_code = "topic_not_found";
            r.error_message = "No topic found with id '" + id + "'" +
                (kind.empty() ? "" : " (kind: " + kind + ")") +
                ". Try search_kb to discover the correct slug.";
            return r;
        }

        nlohmann::json out;
        out["id"]   = id;
        out["kind"] = resolvedKind;
        out["body"] = toValidUtf8(body);
        r.ok = true;
        r.result_json = out.dump();
        return r;
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "read_failed";
        r.error_message = std::string("read_topic threw: ") + e.what();
        return r;
    }
}

// ---- list_commands ------------------------------------------------------

namespace {

// Returns the Markdown content between `## <heading>` and the next `##`
// (or EOF). Case-insensitive heading match. Empty if not found.
std::string extractH2Section(const std::string& doc, std::string_view heading)
{
    if (doc.empty() || heading.empty()) return "";
    auto lowerOf = [](std::string s) {
        for (auto& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return s;
    };
    const std::string lowDoc = lowerOf(doc);
    std::string needle = "## " + lowerOf(std::string(heading));

    std::size_t pos = 0;
    while (pos < lowDoc.size()) {
        auto h = lowDoc.find(needle, pos);
        if (h == std::string::npos) return "";
        // Must be at start of a line.
        if (h != 0 && lowDoc[h - 1] != '\n') { pos = h + 1; continue; }
        // Confirm the heading line ends after the heading text (allowing
        // trailing whitespace) — avoid matching "## Communication X".
        std::size_t lineEnd = lowDoc.find('\n', h);
        if (lineEnd == std::string::npos) lineEnd = lowDoc.size();
        std::string tail = lowDoc.substr(h + needle.size(), lineEnd - h - needle.size());
        bool justWhitespace = true;
        for (char c : tail) {
            if (c != ' ' && c != '\t' && c != '\r') { justWhitespace = false; break; }
        }
        if (!justWhitespace) { pos = h + 1; continue; }
        // Find next `## ` at line start.
        std::size_t next = lineEnd + 1;
        while (next < lowDoc.size()) {
            if (lowDoc.compare(next, 3, "## ") == 0 &&
                (next == 0 || lowDoc[next - 1] == '\n')) break;
            std::size_t nl = lowDoc.find('\n', next);
            if (nl == std::string::npos) { next = lowDoc.size(); break; }
            next = nl + 1;
        }
        return doc.substr(h, next - h);
    }
    return "";
}

} // namespace

std::string ListCommandsTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "group": {
      "type": "string",
      "description": "Exact command-group name as shown in the compact catalog index. Examples: 'Tree Access', 'Object Functions', 'Drawing', 'Strings', 'Statistics'."
    },
    "include_deprecated": {
      "type": "boolean",
      "description": "Pull from the full catalog (includes deprecated) instead of the active-only catalog. Default false."
    }
  },
  "required": ["group"]
})";
}

ToolResult ListCommandsTool::run(std::string_view args_json)
{
    ToolResult r;
    try {
        auto j = nlohmann::json::parse(args_json);
        std::string group = j.value("group", "");
        bool includeDeprecated = j.value("include_deprecated", false);
        if (group.empty()) {
            r.ok = false;
            r.error_code = "missing_group";
            r.error_message = "list_commands expects a 'group' string.";
            return r;
        }

        const char* catalog = includeDeprecated ? "commands" : "commands-active";
        std::string doc = kb::loadCatalog(catalog);
        if (doc.empty()) {
            r.ok = false;
            r.error_code = "catalog_missing";
            r.error_message = std::string("Could not load KNOWLEDGE/catalog/") + catalog + ".md.";
            return r;
        }

        std::string section = extractH2Section(doc, group);
        if (section.empty()) {
            r.ok = false;
            r.error_code = "group_not_found";
            r.error_message = "No '## " + group + "' section in " + catalog + ".md. "
                              "Check the compact catalog index for the exact group name.";
            return r;
        }

        nlohmann::json out;
        out["group"]              = group;
        out["include_deprecated"] = includeDeprecated;
        out["markdown"]           = section;
        r.ok = true;
        r.result_json = out.dump();
        return r;
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "bad_args_json";
        r.error_message = std::string("Couldn't parse args JSON: ") + e.what();
        return r;
    }
}

// ---- list_classes -------------------------------------------------------

std::string ListClassesTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "module": {
      "type": "string",
      "description": "Exact module name as shown in the compact catalog index. Examples: 'AGV', 'Conveyor', 'ProcessFlow', 'Storage', 'Warehousing'. Use 'Core' for unprefixed classes."
    }
  },
  "required": ["module"]
})";
}

ToolResult ListClassesTool::run(std::string_view args_json)
{
    ToolResult r;
    try {
        auto j = nlohmann::json::parse(args_json);
        std::string mod = j.value("module", "");
        if (mod.empty()) {
            r.ok = false;
            r.error_code = "missing_module";
            r.error_message = "list_classes expects a 'module' string.";
            return r;
        }

        std::string doc = kb::loadCatalog("classes");
        if (doc.empty()) {
            r.ok = false;
            r.error_code = "catalog_missing";
            r.error_message = "Could not load KNOWLEDGE/catalog/classes.md.";
            return r;
        }

        // classes.md uses "## <Module> module" for module sections and
        // "## Core (no module prefix)" for the unprefixed bucket. Map common
        // aliases the LLM might pass.
        std::string heading;
        // Match the user's case-insensitive intent to the actual section name.
        if (mod == "Core" || mod == "core" || mod == "(core)") {
            heading = "Core (no module prefix)";
        } else {
            heading = mod + " module";
        }

        std::string section = extractH2Section(doc, heading);
        if (section.empty()) {
            r.ok = false;
            r.error_code = "module_not_found";
            r.error_message = "No section for module '" + mod + "' in classes.md. "
                              "Check the compact catalog index for available module names.";
            return r;
        }

        nlohmann::json out;
        out["module"]   = mod;
        out["markdown"] = section;
        r.ok = true;
        r.result_json = out.dump();
        return r;
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "bad_args_json";
        r.error_message = std::string("Couldn't parse args JSON: ") + e.what();
        return r;
    }
}

} // namespace ModelerAi::tools
