// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/read/kb_tools.h — the two knowledge-base tools the LLM uses to
// pull FlexScript API + command details on demand.
//
// search_kb  — BM25 lookup over the auto-generated class/command index.
//              Returns top-K topic refs (id, name, kind, deprecated,
//              one-line description). Cheap; safe to call freely.
// read_topic — Returns the full Markdown body of a single topic. Use this
//              after search_kb identifies a candidate, or directly when
//              the LLM already knows the symbol name.

#pragma once

#include "tools/tool_base.h"

namespace ModelerAi::tools {

class SearchKbTool : public ITool {
public:
    std::string_view name() const override        { return "search_kb"; }
    std::string_view description() const override {
        return "Searches the FlexSim knowledge base (FlexScript classes + commands "
               "auto-extracted from FlexSim's shipped XML docs). Returns the top "
               "matching topics ranked by relevance. Each hit includes id, name, "
               "kind (class/command), aliases (so you can see WHY a topic matched), "
               "deprecated flag, replaced_by hint, and a one-line description.\n\n"
               "Query strategy that works well:\n"
               "- If you know the symbol name, just use it as the query — exact-name "
               "matching short-circuits the ranker for symbol-shaped queries.\n"
               "- If you only know the behavior, describe it with the verbs from the "
               "domain (e.g. 'move object location', 'set conveyor speed', 'create "
               "AGV path'). The index has snake_case<->camelCase aliases, verb-prefix "
               "aliases (setRank/rank), and reverse-deprecation aliases — so old "
               "command names will find their active replacements.\n"
               "- When the first query returns ambiguous results, REISSUE with a "
               "more specific or differently-phrased query rather than calling "
               "read_topic on everything. 2-3 search_kb calls are cheaper than 5 "
               "read_topic calls.\n"
               "- Use kind=\"class\" or kind=\"command\" to disambiguate when you "
               "already know which surface you want. Use module=\"AGV\" / \"Conveyor\" "
               "/ etc. to scope to a single FlexSim module.\n"
               "- Deprecated topics are excluded by default. Pass include_deprecated=true "
               "only when you specifically need historical/migration info.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::ReadOnly; }
    ToolResult   run(std::string_view args_json) override;
};

class ReadTopicTool : public ITool {
public:
    std::string_view name() const override        { return "read_topic"; }
    std::string_view description() const override {
        return "Returns the full Markdown body of a single KB topic. id is the "
               "topic slug (e.g. 'agv__trailer' for AGV::Trailer, 'createcopy' "
               "for the createcopy command — same slug returned by search_kb). "
               "Kind is optional but recommended ('class' or 'command'); without "
               "it the tool tries both. The body includes method/property signatures, "
               "parameter docs, example code, and deprecation notes where present.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::ReadOnly; }
    ToolResult   run(std::string_view args_json) override;
};

// list_commands(group) — returns the full Markdown table of ACTIVE commands
// in a command group (e.g. "Tree Access", "Object Functions", "Drawing").
// Source: KNOWLEDGE/catalog/commands-active.md. Use this after the compact
// catalog index identifies which group the relevant commands live in.
class ListCommandsTool : public ITool {
public:
    std::string_view name() const override        { return "list_commands"; }
    std::string_view description() const override {
        return "Returns the table of active FlexScript commands in a command group "
               "(name + signature + return type + one-line description). Pass the "
               "exact group name from the compact catalog index (e.g. 'Tree Access', "
               "'Object Functions', 'Drawing'). Use include_deprecated=true to fetch "
               "deprecated commands in the same group instead.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::ReadOnly; }
    ToolResult   run(std::string_view args_json) override;
};

// list_classes(module) — returns the full Markdown table of classes in a
// FlexSim module (e.g. "AGV", "Conveyor", "Core"). Source:
// KNOWLEDGE/catalog/classes.md.
class ListClassesTool : public ITool {
public:
    std::string_view name() const override        { return "list_classes"; }
    std::string_view description() const override {
        return "Returns the table of FlexScript classes in a module "
               "(class name + parent class + summary). Pass the exact module name "
               "from the compact catalog index (e.g. 'AGV', 'Conveyor', 'ProcessFlow') "
               "or 'Core' for unprefixed classes.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::ReadOnly; }
    ToolResult   run(std::string_view args_json) override;
};

} // namespace ModelerAi::tools
