// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// kb/kb_index.h — KB metadata cache + BM25 search.
//
// Loaded once at module init from KNOWLEDGE/INDEX.json (the artifact emitted by
// tools/build-kb.js). The index holds per-topic metadata + per-doc term-
// frequency vectors + global doc-frequency map. search() runs Okapi BM25
// against that. Topic *bodies* are NOT cached here — those are fetched lazily
// by kb_loader when search_kb / read_topic resolves a hit.

#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ModelerAi::kb {

// ---------------------------------------------------------------------------
// Per-topic metadata. Mirrors the `topics` array in INDEX.json verbatim.
// ---------------------------------------------------------------------------
struct TopicMeta {
    std::string              id;          // slug, matches the .md filename stem
    std::string              name;        // human name, e.g. "AGV::Trailer" or "createcopy"
    std::string              kind;        // "class", "command", or "manual"
    std::string              module;      // "AGV", "Conveyor", "" for core
    std::string              group;       // command-group name (commands only)
    std::string              signature;   // one-line signature shown in catalogs / breadcrumb for manual
    std::string              description; // shortdesc / class summary / manual lead-in
    std::vector<std::string> aliases;     // structural + reverse-deprecation aliases
    std::vector<std::string> tags;        // free-form tags (auto-added for manual chapters)
    std::string              replacedBy;  // populated on deprecated commands; "" otherwise
    bool                     deprecated = false;
};

struct SearchResult {
    std::size_t topicIdx = 0; // index into topics() vector
    double      score    = 0.0;
};

struct SearchOptions {
    int                      limit             = 8;
    bool                     includeDeprecated = false;
    std::string              kindFilter;       // "", "class", "command", or "manual"
    std::string              moduleFilter;     // optional module name match (case-insensitive)
    std::vector<std::string> tagFilters;       // optional — topic must include ALL of these tags
};

class KbIndex {
public:
    // Reads <moduleRoot>\KNOWLEDGE\INDEX.json. moduleRoot is expected to end
    // in a backslash. Returns false on missing file / parse failure; in that
    // case the index is empty but valid (search() returns nothing).
    bool load(const std::string& moduleRoot);

    bool                          loaded() const { return loaded_; }
    std::size_t                   size()   const { return topics_.size(); }
    const std::vector<TopicMeta>& topics() const { return topics_; }
    const TopicMeta*              find(std::string_view id) const;

    // Run BM25 search. Returns ranked SearchResults (highest score first),
    // truncated to opts.limit. Empty query returns empty result vector.
    std::vector<SearchResult> search(std::string_view query,
                                     const SearchOptions& opts) const;

private:
    // Tokenize the same way build-kb.js does: lowercase, [a-z0-9_]+.
    static std::vector<std::string> tokenize(std::string_view s);

    bool                                                   loaded_ = false;
    std::vector<TopicMeta>                                 topics_;
    std::unordered_map<std::string, std::size_t>           idIndex_;     // id -> topics_[i]
    std::unordered_map<std::string, std::size_t>           docFreq_;     // term -> # docs containing
    // BM25F: per-doc *weighted* tf (sum across fields, scaled by FIELD_WEIGHTS
    // upstream in build-kb.js). The C++ side just trusts the weights baked in.
    std::vector<std::unordered_map<std::string, double>>   docTf_;
    std::vector<double>                                    docLen_;      // weighted doc length
    double                                                 avgDocLen_ = 0.0;

    // Exact-name short-circuit map. Key is a lowercased token; value is a
    // list of topics whose name (or one of whose aliases) equals that token
    // verbatim. Most queries that ARE a known symbol go through this path
    // and skip BM25 entirely — much faster, and removes a class of failures
    // where the rerank-style scoring nudges the wrong topic to the top.
    std::unordered_map<std::string, std::vector<std::size_t>> exactNameIdx_;
};

} // namespace ModelerAi::kb
