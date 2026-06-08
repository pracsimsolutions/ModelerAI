// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/schema_store.h — in-process cache of per-class property
// schemas read from KNOWLEDGE/schemas/<Class>.json.
//
// Lazy load: the first time a class is requested, the JSON is read from
// <module install dir>/KNOWLEDGE/schemas/<Class>.json and parsed into a
// PropertyEntry table keyed by name. Subsequent lookups hit the cache.
//
// Source of truth is the modelerai_extract_class_schema tool that walks
// function_s(class, "enumerateProperties") and emits the JSON files. To
// regenerate the on-disk corpus after a FlexSim version bump, run the
// forobjecttreeunder loop documented in KNOWLEDGE/schemas/README.md and
// commit the diff.

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace ModelerAi {
namespace Schema {

struct PropertyEntry {
    std::string name;
    std::string type;             // "Vec3Property" / "ComboProperty" / ...
    std::string value_kind;       // "vec3" / "enum" / "number" / "boolean" / ...
    std::string source;           // "global" / "class"
    std::string localized_name;
    std::string category;
    std::vector<std::string> components;                       // composite folding
    std::string options_kind;                                  // "static" | "dynamic" | ""
    std::vector<std::pair<std::string, long long>> options;    // static-only
    std::string options_path;                                  // dynamic-only
};

struct ClassSchema {
    std::string class_name;
    std::string extracted_at;
    // Lookup by property name. Insertion order isn't preserved; iterate
    // `property_order` if you need stable ordering for output.
    std::unordered_map<std::string, PropertyEntry> by_name;
    std::vector<std::string> property_order;
};

// Returns a pointer into the cache. nullptr if the class has no schema
// file on disk (or the JSON failed to parse). Pointer is stable for the
// lifetime of the process unless clearCache() is called.
const ClassSchema* get(const std::string& className);

// Look up one property entry by class + property name. Convenience for
// the common set_property / get_property gating path.
const PropertyEntry* findProperty(const std::string& className,
                                   const std::string& propertyName);

// Wipe the cache. Useful if the schemas/ directory was just regenerated
// during a FlexSim session and the user wants the new schemas without a
// DLL reload. Not currently bound to a tool; expose if needed.
void clearCache();

// Diagnostic: how many class schemas are currently in the cache.
size_t cachedCount();

} // namespace Schema
} // namespace ModelerAi
