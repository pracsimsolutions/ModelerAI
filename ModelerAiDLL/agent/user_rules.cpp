// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "agent/user_rules.h"
#include "paths.h"

#include <fstream>
#include <sstream>
#include <string>

namespace ModelerAi::agent {

namespace {

constexpr const char* kTemplate =
    "# ModelerAI - my rules\n"
    "\n"
    "These are durable instructions ModelerAI follows on every turn. Edit\n"
    "freely; changes take effect on the next message you send.\n"
    "\n"
    "Examples to delete and replace with your own:\n"
    "\n"
    "- Use meters for all dimensions; never feet.\n"
    "- Name Sources with prefix SRC_ and Sinks with prefix SNK_.\n"
    "- When suggesting object placement, leave at least 2 m of clearance\n"
    "  between adjacent objects.\n"
    "- Default arrival distribution for Sources is exponential(60) unless\n"
    "  I specify otherwise.\n"
    "- Always explain what you're doing in plain English before showing\n"
    "  any FlexScript.\n";

void writeTemplateIfMissing(const std::string& path)
{
    std::ifstream check(path);
    if (check.good()) return;  // exists, leave alone
    std::ofstream out(path, std::ios::binary);
    if (!out) return;
    out.write(kTemplate, std::char_traits<char>::length(kTemplate));
}

std::string trimRight(std::string s)
{
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' ||
                          s.back() == '\r' || s.back() == '\n')) {
        s.pop_back();
    }
    return s;
}

} // namespace

std::string userRulesPath()
{
    std::string dir = paths::appDataDir();
    if (dir.empty()) return "";
    return dir + "rules.md";
}

std::string loadUserRules()
{
    std::string path = userRulesPath();
    if (path.empty()) return "";
    writeTemplateIfMissing(path);

    std::ifstream f(path, std::ios::binary);
    if (!f) return "";
    std::ostringstream os;
    os << f.rdbuf();
    return trimRight(os.str());
}

} // namespace ModelerAi::agent
