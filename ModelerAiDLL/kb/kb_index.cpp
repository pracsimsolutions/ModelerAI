// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "kb/kb_index.h"
#include "third_party/json.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <fstream>
#include <sstream>
#include <unordered_set>

namespace ModelerAi::kb {

namespace {

// BM25 knobs — standard textbook values. Worth revisiting once we have
// real-query telemetry, but these are sane defaults for short metadata
// indexes like ours (avgDocLen ≈ 16 tokens).
constexpr double kBm25K1 = 1.5;
constexpr double kBm25B  = 0.75;

bool iEquals(std::string_view a, std::string_view b)
{
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (std::tolower(static_cast<unsigned char>(a[i])) !=
            std::tolower(static_cast<unsigned char>(b[i]))) return false;
    }
    return true;
}

} // namespace

std::vector<std::string> KbIndex::tokenize(std::string_view s)
{
    std::vector<std::string> out;
    std::string cur;
    cur.reserve(16);
    for (char c : s) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (std::isalnum(uc) || c == '_') {
            cur.push_back(static_cast<char>(std::tolower(uc)));
        } else if (!cur.empty()) {
            out.push_back(std::move(cur));
            cur.clear();
        }
    }
    if (!cur.empty()) out.push_back(std::move(cur));
    return out;
}

bool KbIndex::load(const std::string& moduleRoot)
{
    loaded_   = false;
    topics_.clear();
    idIndex_.clear();
    docFreq_.clear();
    docTf_.clear();
    docLen_.clear();
    exactNameIdx_.clear();
    avgDocLen_ = 0.0;

    if (moduleRoot.empty()) return false;
    std::string path = moduleRoot + "KNOWLEDGE\\INDEX.json";

    std::ifstream f(path, std::ios::binary);
    if (!f) return false;

    std::ostringstream os;
    os << f.rdbuf();
    std::string text = os.str();
    if (text.empty()) return false;

    nlohmann::json j;
    try {
        j = nlohmann::json::parse(text);
    } catch (...) {
        return false;
    }

    if (!j.contains("topics") || !j["topics"].is_array()) return false;

    const auto& jt = j["topics"];
    topics_.reserve(jt.size());
    for (const auto& it : jt) {
        TopicMeta m;
        m.id          = it.value("id", "");
        m.name        = it.value("name", "");
        m.kind        = it.value("kind", "");
        m.module      = it.value("module", "");
        m.group       = it.value("group", "");
        m.signature   = it.value("signature", "");
        m.description = it.value("description", "");
        m.deprecated  = it.value("deprecated", false);
        if (it.contains("replacedBy") && it["replacedBy"].is_string()) {
            m.replacedBy = it["replacedBy"].get<std::string>();
        }
        if (it.contains("aliases") && it["aliases"].is_array()) {
            for (const auto& a : it["aliases"]) {
                if (a.is_string()) m.aliases.push_back(a.get<std::string>());
            }
        }
        if (it.contains("tags") && it["tags"].is_array()) {
            for (const auto& a : it["tags"]) {
                if (a.is_string()) m.tags.push_back(a.get<std::string>());
            }
        }
        if (m.id.empty()) continue;
        idIndex_[m.id] = topics_.size();
        topics_.push_back(std::move(m));
    }

    // Exact-name short-circuit. Index by the topic's name AND each alias,
    // lowercased + tokenized (so "AGV::Trailer" indexes under "agv" AND
    // "trailer"; "createcopy" indexes under just "createcopy"). Multiple
    // topics can share a token — the value vector preserves all candidates.
    for (std::size_t i = 0; i < topics_.size(); ++i) {
        const auto& m = topics_[i];
        auto addKey = [&](const std::string& key) {
            auto toks = tokenize(key);
            // Whole-symbol key — for queries that match the symbol verbatim.
            std::string lower;
            lower.reserve(key.size());
            for (char c : key) lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            if (!lower.empty()) exactNameIdx_[lower].push_back(i);
            // Single-token keys — caught by exact-token queries.
            if (toks.size() == 1 && toks[0] != lower) {
                exactNameIdx_[toks[0]].push_back(i);
            }
        };
        if (!m.name.empty()) addKey(m.name);
        for (const auto& a : m.aliases) {
            if (!a.empty()) addKey(a);
        }
    }

    // BM25 stats. Tolerate missing/partial — leave the docTf/docLen arrays
    // empty, search() will return [] which is the right degraded behavior.
    if (j.contains("bm25") && j["bm25"].is_object()) {
        const auto& jbm = j["bm25"];
        avgDocLen_ = jbm.value("avgdl", 0.0);
        if (jbm.contains("df") && jbm["df"].is_object()) {
            for (auto it = jbm["df"].begin(); it != jbm["df"].end(); ++it) {
                docFreq_[it.key()] = it.value().get<std::size_t>();
            }
        }
        if (jbm.contains("docs") && jbm["docs"].is_array()) {
            // Map id -> tf table so we can align with topics_ (which we
            // filtered above). Order in INDEX.json should match but be
            // defensive — id-based join is cheap.
            std::unordered_map<std::string, std::size_t> docByIdJson;
            for (std::size_t k = 0; k < jbm["docs"].size(); ++k) {
                docByIdJson[jbm["docs"][k].value("id", "")] = k;
            }
            docTf_.resize(topics_.size());
            docLen_.assign(topics_.size(), 0.0);
            for (std::size_t i = 0; i < topics_.size(); ++i) {
                auto it = docByIdJson.find(topics_[i].id);
                if (it == docByIdJson.end()) continue;
                const auto& jd = jbm["docs"][it->second];
                docLen_[i] = jd.value("len", 0.0);
                if (jd.contains("tf") && jd["tf"].is_object()) {
                    auto& tf = docTf_[i];
                    for (auto jt2 = jd["tf"].begin(); jt2 != jd["tf"].end(); ++jt2) {
                        // Per-doc tf is now a weighted float (BM25F). Older v1
                        // INDEX.json files stored integers — accept either.
                        if (jt2.value().is_number()) {
                            tf[jt2.key()] = jt2.value().get<double>();
                        }
                    }
                }
            }
        }
    }

    loaded_ = true;
    return true;
}

const TopicMeta* KbIndex::find(std::string_view id) const
{
    auto it = idIndex_.find(std::string(id));
    if (it == idIndex_.end()) return nullptr;
    return &topics_[it->second];
}

std::vector<SearchResult> KbIndex::search(std::string_view query,
                                          const SearchOptions& opts) const
{
    std::vector<SearchResult> out;
    if (topics_.empty()) return out;
    if (docTf_.size() != topics_.size()) return out; // index missing BM25 data

    auto qTokens = tokenize(query);
    if (qTokens.empty()) return out;

    int limit = opts.limit > 0 ? opts.limit : 8;

    auto passesFilters = [&](const TopicMeta& m) {
        if (!opts.includeDeprecated && m.deprecated) return false;
        if (!opts.kindFilter.empty()   && !iEquals(opts.kindFilter,   m.kind))   return false;
        if (!opts.moduleFilter.empty() && !iEquals(opts.moduleFilter, m.module)) return false;
        // ALL tag filters must match (intersection semantics — useful for
        // scoping a manual search to a specific chapter via tag).
        for (const auto& wanted : opts.tagFilters) {
            bool found = false;
            for (const auto& have : m.tags) {
                if (iEquals(wanted, have)) { found = true; break; }
            }
            if (!found) return false;
        }
        return true;
    };

    // -----------------------------------------------------------------
    // Exact-name short-circuit. If the entire query (lowercased) matches a
    // known symbol verbatim, or each individual query token matches a known
    // symbol verbatim, return those topics first with a high score. This
    // covers two important cases:
    //   - User pastes a symbol they read somewhere ("AGV::Trailer", "setrank")
    //   - LLM tool-calls with a symbol it remembers from prior context
    // For multi-token queries that don't match verbatim, fall through to BM25.
    // -----------------------------------------------------------------
    std::unordered_set<std::size_t> shortCircuit;

    {
        // Whole-query lowercased lookup (handles "AGV::Trailer", "Object.location").
        std::string qLower;
        qLower.reserve(query.size());
        for (char c : query) qLower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
        auto it = exactNameIdx_.find(qLower);
        if (it != exactNameIdx_.end()) {
            for (auto idx : it->second) {
                if (passesFilters(topics_[idx])) shortCircuit.insert(idx);
            }
        }
    }
    // Per-token exact match (handles single-word queries like "setrank" or
    // multi-word where each word is a symbol — rare but cheap to check).
    for (const auto& t : qTokens) {
        auto it = exactNameIdx_.find(t);
        if (it == exactNameIdx_.end()) continue;
        for (auto idx : it->second) {
            if (passesFilters(topics_[idx])) shortCircuit.insert(idx);
        }
    }

    if (!shortCircuit.empty()) {
        // Emit short-circuit hits first with a strong base score so the
        // subsequent BM25 pass can't beat them. Score still slightly orders
        // by deprecation state.
        for (auto idx : shortCircuit) {
            double score = 100.0;
            if (topics_[idx].deprecated) score -= 1.0;
            out.push_back({ idx, score });
        }
    }

    // -----------------------------------------------------------------
    // BM25F over the rest. Per-doc tf is already field-weighted (set by
    // build-kb.js using FIELD_WEIGHTS), so this loop is identical to plain
    // BM25 mathematically — the weighting lives in the data, not the code.
    // We skip topics already in shortCircuit to avoid double-scoring.
    // -----------------------------------------------------------------
    std::unordered_set<std::string> qSet(qTokens.begin(), qTokens.end());

    const double N     = static_cast<double>(topics_.size());
    const double avgdl = avgDocLen_ > 0.0 ? avgDocLen_ : 1.0;

    for (std::size_t i = 0; i < topics_.size(); ++i) {
        if (shortCircuit.count(i)) continue;
        const TopicMeta& m = topics_[i];
        if (!passesFilters(m)) continue;

        double score = 0.0;
        const auto& tf = docTf_[i];
        const double dl = docLen_[i] > 0.0 ? docLen_[i] : 1.0;

        for (const auto& term : qSet) {
            auto tfIt = tf.find(term);
            if (tfIt == tf.end()) continue;
            auto dfIt = docFreq_.find(term);
            double df = dfIt == docFreq_.end() ? 0.5 : static_cast<double>(dfIt->second);
            double idf = std::log(1.0 + (N - df + 0.5) / (df + 0.5));
            double f = tfIt->second;
            double denom = f + kBm25K1 * (1.0 - kBm25B + kBm25B * (dl / avgdl));
            score += idf * (f * (kBm25K1 + 1.0) / denom);
        }

        // Tiny deprecated penalty — keeps active recommendations above their
        // deprecated counterparts when the score is otherwise close.
        if (m.deprecated) score -= 0.5;

        if (score > 0.0) {
            out.push_back({ i, score });
        }
    }

    std::sort(out.begin(), out.end(),
              [](const SearchResult& a, const SearchResult& b) {
                  return a.score > b.score;
              });

    if (static_cast<int>(out.size()) > limit) out.resize(limit);
    return out;
}

} // namespace ModelerAi::kb
