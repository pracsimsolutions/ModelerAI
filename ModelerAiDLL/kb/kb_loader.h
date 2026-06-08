// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// kb/kb_loader.h — filesystem helpers for the KB.
//
// Three lookups:
//   loadTopicBody(id)             — hand-written topic at KNOWLEDGE/topics/<id>.md
//   loadTopicBody(kind, id)       — auto-generated topic at KNOWLEDGE/topics/<kind>/<id>.md
//                                   (kind ∈ {"classes", "commands", "patterns", …})
//   loadCatalog(name)             — KNOWLEDGE/catalog/<name>.md (e.g. "classes",
//                                   "commands-active"). Used by the prompt assembler
//                                   to fold the always-on symbol tables into the
//                                   cached system prefix.
//
// All readers strip the YAML frontmatter so the body the LLM sees is just
// Markdown.

#pragma once

#include <string>
#include <string_view>

namespace ModelerAi::kb {

// Reads KNOWLEDGE/topics/<topicId>.md (hand-written layer).
std::string loadTopicBody(std::string_view topicId);

// Reads KNOWLEDGE/topics/<kind>/<topicId>.md (auto-generated layer).
// kind = "classes" -> uses the topic slug as the filename stem (which is the
// FlexScript class name with "::" rewritten as "__", e.g. "AGV__Trailer").
std::string loadTopicBody(std::string_view kind, std::string_view topicId);

// Reads KNOWLEDGE/catalog/<name>.md (always-on symbol tables).
std::string loadCatalog(std::string_view name);

// Wraps a topic body in the "--- topic: <id> ---" header the prompt
// assembler expects. Empty when the topic body is empty.
std::string buildKbBlockForTopic(std::string_view topicId);

} // namespace ModelerAi::kb
