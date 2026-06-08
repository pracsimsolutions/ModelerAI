// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/identifier_validator.cpp — implementation of the FlexScript
// identifier validator. See identifier_validator.h for design notes.

#include "commands/identifier_validator.h"
#include "bootstrap.h"
#include "kb/kb_index.h"

#include <cctype>
#include <mutex>
#include <regex>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace ModelerAi {

namespace {

// Catalog state. Populated by rebuildIdentifierCatalog(); read by
// validateIdentifiers(). Guarded by g_mu so the (rare) rebuild can
// race safely with concurrent validation calls.
std::mutex                                           g_mu;
std::unordered_set<std::string>                      g_known;
std::unordered_map<std::string, std::string>         g_deprecated;

// FlexScript keywords, primitive type names, and reference types that
// can be call-shaped (constructors, casts) but should NOT be flagged
// against the KB catalog. Validating these would generate false
// positives — they're language-level forms, not library calls.
const std::unordered_set<std::string>& builtinKeywords()
{
    static const std::unordered_set<std::string> kKW = {
        // Control flow
        "if", "else", "while", "do", "for", "switch", "case", "default",
        "return", "break", "continue", "goto",
        // Literals + casts
        "true", "false", "null", "this", "super",
        "new", "delete", "sizeof", "typeof", "as",
        // Primitive types
        "int", "double", "float", "char", "bool", "void", "string",
        "const", "auto", "static", "extern",
        // FlexScript reference types (constructor-callable; also used as
        // declared types so the validator must not flag them).
        "treenode", "Object", "Variant", "Array", "Map", "Set", "List",
        "Vec2", "Vec3", "Vec4", "Quat", "Mat4",
        "Table", "Group", "NodeRef", "Token", "TaskSequence", "Window",
        // Common FlexSim 3D object class names — these can appear in
        // type-cast positions like `(Source)current` or as declared
        // types. Their catalog entries cover the create-shape; here
        // we just skip type-position usage.
        "TaskExecuter", "Source", "Queue", "Processor", "Sink",
        "Conveyor", "Combiner", "Separator", "MultiProcessor",
    };
    return kKW;
}

// Build g_known + g_deprecated from the global KB. Called under g_mu.
void buildCatalogLocked()
{
    g_known.clear();
    g_deprecated.clear();

    auto* idx = bootstrap::kbIndex();
    if (!idx || !idx->loaded()) return;

    for (const auto& t : idx->topics()) {
        // Only commands and classes — manual topics are conceptual
        // articles, not callable identifiers.
        if (t.kind != "command" && t.kind != "class") continue;
        if (t.name.empty()) continue;

        g_known.insert(t.name);
        for (const auto& a : t.aliases) {
            if (!a.empty()) g_known.insert(a);
        }

        if (t.deprecated) {
            g_deprecated[t.name] = t.replacedBy;
            for (const auto& a : t.aliases) {
                if (!a.empty()) g_deprecated[a] = t.replacedBy;
            }
        }
    }
}

// Replace comments and string literals with whitespace (preserving
// newlines) so the column-position math downstream is preserved and
// no identifier inside a string accidentally gets flagged.
std::string stripCommentsAndStrings(const std::string& src)
{
    std::string out;
    out.reserve(src.size());
    size_t i = 0;
    while (i < src.size()) {
        char c = src[i];
        // Line comment
        if (c == '/' && i + 1 < src.size() && src[i + 1] == '/') {
            while (i < src.size() && src[i] != '\n') { out.push_back(' '); ++i; }
            continue;
        }
        // Block comment
        if (c == '/' && i + 1 < src.size() && src[i + 1] == '*') {
            out.append("  ", 2); i += 2;
            while (i + 1 < src.size() && !(src[i] == '*' && src[i + 1] == '/')) {
                out.push_back(src[i] == '\n' ? '\n' : ' ');
                ++i;
            }
            if (i + 1 < src.size()) { out.append("  ", 2); i += 2; }
            else                    { while (i < src.size()) { out.push_back(src[i] == '\n' ? '\n' : ' '); ++i; } }
            continue;
        }
        // String literal — double or single quote
        if (c == '"' || c == '\'') {
            char quote = c;
            out.push_back(' '); ++i;
            while (i < src.size() && src[i] != quote) {
                if (src[i] == '\\' && i + 1 < src.size()) {
                    out.append("  ", 2); i += 2;
                } else {
                    out.push_back(src[i] == '\n' ? '\n' : ' ');
                    ++i;
                }
            }
            if (i < src.size()) { out.push_back(' '); ++i; }
            continue;
        }
        out.push_back(c);
        ++i;
    }
    return out;
}

// Find user-declared functions in the script and seed them into a
// local name set so subsequent uses don't get flagged as unknown.
// Pattern: <type> <name> ( ... ) { ... }
std::unordered_set<std::string> localFunctionDecls(const std::string& src)
{
    std::unordered_set<std::string> out;
    static const std::regex re(
        R"((?:^|[^A-Za-z0-9_])(treenode|int|double|float|string|Variant|void|bool|char|Object|Array|Map|Vec[234]|Table|Group)\s+([A-Za-z_]\w*)\s*\([^)]*\)\s*\{)",
        std::regex::ECMAScript);
    auto begin = std::sregex_iterator(src.begin(), src.end(), re);
    auto end   = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        out.insert((*it)[2].str());
    }
    return out;
}

// 1-based line number for a character offset.
int lineNoOf(const std::string& s, size_t offset)
{
    int line = 1;
    size_t up = offset < s.size() ? offset : s.size();
    for (size_t i = 0; i < up; ++i) {
        if (s[i] == '\n') ++line;
    }
    return line;
}

// Cascade syntactic recovery (Aider-style fuzzy match before re-prompting).
// When an identifier isn't in the catalog, try common deterministic
// normalizations and check each against the known set. The first hit
// is returned as "did_you_mean" — high enough confidence that the AI
// shouldn't have to think; it can just substitute. Order matters:
// cheaper / higher-confidence transforms first.
//
// Caller must hold g_mu.
std::string findDidYouMeanLocked(const std::string& name)
{
    if (g_known.count(name)) return ""; // shouldn't happen but safe

    // 1. Lowercase — handles createInstance → createinstance (real
    //    deprecated). FlexSim's command names are largely lowercase.
    {
        std::string lo = name;
        for (auto& c : lo) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        if (lo != name && g_known.count(lo)) return lo;
    }

    // 2. camelCase → snake_case — e.g. createObject → create_object.
    //    Catches AI's tendency to camelCase modelerai_* tool names.
    {
        std::string snake;
        snake.reserve(name.size() + 4);
        for (size_t i = 0; i < name.size(); ++i) {
            char c = name[i];
            if (std::isupper(static_cast<unsigned char>(c)) && i > 0 &&
                std::isalnum(static_cast<unsigned char>(name[i - 1]))) {
                snake.push_back('_');
            }
            snake.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
        if (snake != name && g_known.count(snake)) return snake;
    }

    // 3. snake_case → camelCase — for the other direction.
    {
        std::string camel;
        camel.reserve(name.size());
        bool capitalizeNext = false;
        for (char c : name) {
            if (c == '_') { capitalizeNext = true; continue; }
            if (capitalizeNext) {
                camel.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
                capitalizeNext = false;
            } else {
                camel.push_back(c);
            }
        }
        if (camel != name && g_known.count(camel)) return camel;
    }

    // 4. lowercased + snake_case → catches mixed-up AI outputs like
    //    CreateObject → create_object.
    {
        std::string snake;
        snake.reserve(name.size() + 4);
        for (size_t i = 0; i < name.size(); ++i) {
            char c = name[i];
            if (std::isupper(static_cast<unsigned char>(c)) && i > 0 &&
                std::isalnum(static_cast<unsigned char>(name[i - 1]))) {
                snake.push_back('_');
            }
            snake.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        }
        // Try as modelerai_* tool name
        std::string asTool = "modelerai_" + snake;
        if (g_known.count(asTool)) return asTool;
    }

    return "";
}

} // anonymous

void rebuildIdentifierCatalog()
{
    std::lock_guard<std::mutex> lk(g_mu);
    buildCatalogLocked();
}

ValidationResult validateIdentifiers(const std::string& script)
{
    ValidationResult result;

    // Catalog empty (KB not loaded) → fail-open: skip validation
    // rather than block every script.
    {
        std::lock_guard<std::mutex> lk(g_mu);
        if (g_known.empty()) return result;
    }

    std::string stripped = stripCommentsAndStrings(script);
    std::unordered_set<std::string> locals = localFunctionDecls(stripped);
    const auto& kw = builtinKeywords();

    // Identifier followed by '(' that is NOT preceded by an identifier
    // char, '.', or ':' (the last two protect method and scoped calls).
    static const std::regex re(
        R"((^|[^A-Za-z0-9_.:])([A-Za-z_][A-Za-z0-9_]*)\s*\()",
        std::regex::ECMAScript);

    std::unordered_set<std::string> alreadyReported;
    std::lock_guard<std::mutex> lk(g_mu);

    auto begin = std::sregex_iterator(stripped.begin(), stripped.end(), re);
    auto end   = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        std::string name = (*it)[2].str();
        if (name.empty()) continue;
        if (kw.count(name))     continue;
        if (locals.count(name)) continue;
        // modelerai_* curated commands — let the bridge resolve. They
        // ARE in the catalog as classes/commands too once the KB
        // re-runs, but cheap to short-circuit.
        if (name.rfind("modelerai_", 0) == 0)  continue;
        if (name.rfind("ModelerAi",  0) == 0)  continue;
        // Already flagged once in this script — don't duplicate.
        if (!alreadyReported.insert(name).second) continue;

        int line = lineNoOf(stripped, static_cast<size_t>(it->position(2)));

        auto dit = g_deprecated.find(name);
        if (dit != g_deprecated.end()) {
            ValidationFinding f;
            f.name        = name;
            f.status      = ValidationFinding::Deprecated;
            f.replacedBy  = dit->second;
            f.approx_line = line;
            result.findings.push_back(std::move(f));
            continue;
        }
        if (g_known.count(name)) continue;

        ValidationFinding f;
        f.name        = name;
        f.status      = ValidationFinding::Unknown;
        f.approx_line = line;
        // Cascade syntactic recovery — if a deterministic
        // normalization (case / snake↔camel / modelerai_*-prefix)
        // lands on a known identifier, name it directly. Saves a
        // round trip when the AI just got the case wrong.
        f.didYouMean  = findDidYouMeanLocked(name);
        result.findings.push_back(std::move(f));
    }
    return result;
}

} // namespace ModelerAi
