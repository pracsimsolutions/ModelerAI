// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/schema_store.cpp — implementation of the per-class schema
// cache. Reads <module install dir>/KNOWLEDGE/schemas/<Class>.json on
// demand. Misses are cached as "no schema" (an empty ClassSchema with a
// sentinel value_kind on lookup) so we don't reread a missing file every
// call.

#include "module.h"
#include "commands/schema_store.h"
#include "paths.h"
#include "third_party/json.h"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>

namespace ModelerAi {
namespace Schema {

namespace {

// Holds either a parsed ClassSchema or a marker that we tried and missed.
struct CacheEntry {
    bool present = false;
    ClassSchema schema;
};

std::unordered_map<std::string, CacheEntry> g_cache;
std::mutex g_cacheMutex;

std::string schemasDir()
{
    std::string installDir = paths::moduleInstallDir();
    if (installDir.empty()) return "";
    return installDir + "KNOWLEDGE\\schemas\\";
}

// Module classes are namespaced with "::" but on-disk filenames are bare
// (Path.json, not AGV__Path.json — the extractor names files by the
// terminal segment after the last "::"). Mirror that convention here so
// schemaFilePath("AGV::Path") resolves the same file the extractor wrote.
std::string bareClassName(const std::string& className)
{
    size_t pos = className.rfind("::");
    if (pos == std::string::npos) return className;
    return className.substr(pos + 2);
}

bool parseSchemaJson(const nlohmann::json& j, ClassSchema& out)
{
    if (!j.is_object()) return false;
    out.class_name   = j.value("class",        std::string(""));
    out.extracted_at = j.value("extracted_at", std::string(""));

    if (!j.contains("properties") || !j["properties"].is_array()) return false;
    for (const auto& p : j["properties"]) {
        if (!p.is_object()) continue;
        PropertyEntry e;
        e.name           = p.value("name",            std::string(""));
        if (e.name.empty()) continue;
        e.type           = p.value("type",            std::string(""));
        e.value_kind     = p.value("value_kind",      std::string(""));
        e.source         = p.value("source",          std::string(""));
        e.localized_name = p.value("localized_name",  std::string(""));
        e.category       = p.value("category",        std::string(""));

        if (p.contains("components") && p["components"].is_array()) {
            for (const auto& c : p["components"]) {
                if (c.is_string()) e.components.push_back(c.get<std::string>());
            }
        }

        if (p.contains("options_kind") && p["options_kind"].is_string()) {
            e.options_kind = p["options_kind"].get<std::string>();
        }
        if (p.contains("options") && p["options"].is_array()) {
            for (const auto& o : p["options"]) {
                if (!o.is_object()) continue;
                std::string oname = o.value("name", std::string(""));
                long long   ovalue = o.value("value", (long long)0);
                if (!oname.empty()) e.options.emplace_back(oname, ovalue);
            }
        }
        if (p.contains("options_path") && p["options_path"].is_string()) {
            e.options_path = p["options_path"].get<std::string>();
        }

        out.property_order.push_back(e.name);
        out.by_name.emplace(e.name, std::move(e));
    }
    return true;
}

// Read + parse the on-disk schema. Returns false if the file is missing
// or fails to parse (caller caches the miss so we don't retry every call).
bool loadFromDisk(const std::string& className, ClassSchema& out)
{
    std::string dir = schemasDir();
    if (dir.empty()) return false;

    std::filesystem::path filePath = std::filesystem::path(dir)
                                     / (bareClassName(className) + ".json");
    std::error_code ec;
    if (!std::filesystem::exists(filePath, ec)) return false;

    std::ifstream f(filePath, std::ios::in | std::ios::binary);
    if (!f.is_open()) return false;
    std::ostringstream buf;
    buf << f.rdbuf();
    std::string raw = buf.str();
    if (raw.empty()) return false;

    nlohmann::json j;
    try {
        j = nlohmann::json::parse(raw);
    } catch (...) {
        return false;
    }
    return parseSchemaJson(j, out);
}

} // namespace

const ClassSchema* get(const std::string& className)
{
    if (className.empty()) return nullptr;
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    auto it = g_cache.find(className);
    if (it != g_cache.end()) {
        return it->second.present ? &it->second.schema : nullptr;
    }
    CacheEntry entry;
    entry.present = loadFromDisk(className, entry.schema);
    auto [ins, _] = g_cache.emplace(className, std::move(entry));
    return ins->second.present ? &ins->second.schema : nullptr;
}

const PropertyEntry* findProperty(const std::string& className,
                                   const std::string& propertyName)
{
    const ClassSchema* schema = get(className);
    if (!schema) return nullptr;
    auto it = schema->by_name.find(propertyName);
    if (it == schema->by_name.end()) return nullptr;
    return &it->second;
}

void clearCache()
{
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    g_cache.clear();
}

size_t cachedCount()
{
    std::lock_guard<std::mutex> lock(g_cacheMutex);
    size_t n = 0;
    for (const auto& kv : g_cache) if (kv.second.present) ++n;
    return n;
}

} // namespace Schema
} // namespace ModelerAi
