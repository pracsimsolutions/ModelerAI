// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "kb/kb_loader.h"
#include "paths.h"

#include <fstream>
#include <sstream>
#include <string>

namespace ModelerAi::kb {

namespace {

// Strip a YAML frontmatter block (--- ... ---) from the start of `text`.
// Returns the slice after the closing fence. If no frontmatter present,
// returns text unchanged.
std::string_view stripFrontmatter(std::string_view text)
{
    if (text.substr(0, 4) != "---\n" && text.substr(0, 5) != "---\r\n") return text;
    auto afterOpen = text.find('\n');
    if (afterOpen == std::string_view::npos) return text;
    auto close = text.find("\n---", afterOpen);
    if (close == std::string_view::npos) return text;
    auto afterClose = text.find('\n', close + 4);
    if (afterClose == std::string_view::npos) return std::string_view{};
    return text.substr(afterClose + 1);
}

std::string readFileStrippedFrontmatter(const std::string& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) return "";

    std::ostringstream os;
    os << f.rdbuf();
    std::string text = os.str();

    auto body = stripFrontmatter(text);
    while (!body.empty() && (body.front() == '\n' || body.front() == '\r')) {
        body.remove_prefix(1);
    }
    return std::string(body);
}

std::string readFileRaw(const std::string& path)
{
    std::ifstream f(path, std::ios::binary);
    if (!f) return "";
    std::ostringstream os;
    os << f.rdbuf();
    return os.str();
}

} // namespace

std::string loadTopicBody(std::string_view topicId)
{
    std::string installDir = paths::moduleInstallDir();
    if (installDir.empty()) return "";

    std::string path = installDir + "KNOWLEDGE\\topics\\";
    path.append(topicId);
    path.append(".md");
    return readFileStrippedFrontmatter(path);
}

std::string loadTopicBody(std::string_view kind, std::string_view topicId)
{
    std::string installDir = paths::moduleInstallDir();
    if (installDir.empty()) return "";

    std::string path = installDir + "KNOWLEDGE\\topics\\";
    path.append(kind);
    path.append("\\");
    path.append(topicId);
    path.append(".md");
    return readFileStrippedFrontmatter(path);
}

std::string loadCatalog(std::string_view name)
{
    std::string installDir = paths::moduleInstallDir();
    if (installDir.empty()) return "";

    std::string path = installDir + "KNOWLEDGE\\catalog\\";
    path.append(name);
    path.append(".md");
    // Catalog files have no frontmatter — read raw.
    return readFileRaw(path);
}

std::string buildKbBlockForTopic(std::string_view topicId)
{
    std::string body = loadTopicBody(topicId);
    if (body.empty()) return "";
    std::string out;
    out.reserve(body.size() + 64);
    out.append("--- topic: ");
    out.append(topicId);
    out.append(" ---\n");
    out.append(body);
    return out;
}

} // namespace ModelerAi::kb
