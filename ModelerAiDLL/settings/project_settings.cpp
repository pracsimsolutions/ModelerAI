// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"                          // FlexSim SDK; must precede json.h
#include "settings/project_settings.h"
#include "bridge/log.h"
#include "third_party/json.h"

#include <cstring>

namespace ModelerAi::settings {

namespace {

// Locate the in-model ModelerAI object (the one addmodelerai creates a
// copy of from the library template). Returns nullptr if addmodelerai
// hasn't run yet on the current model, or if there's no model loaded.
treenode findInModelObject()
{
    try {
        treenode m = model();
        if (!m) return nullptr;
        return m->find("Tools/ModelerAI/ModelerAI");
    } catch (...) {
        return nullptr;
    }
}

// Find a direct child by name. Returns nullptr if absent. Avoids find()
// because find() walks subtrees and we only want a direct child match.
treenode findChild(treenode parent, const char* name)
{
    if (!parent || !name) return nullptr;
    try {
        int n = (int)content(parent);
        for (int i = 1; i <= n; ++i) {
            treenode c = rank(parent, i);
            if (!c) continue;
            const char* cn = c->getName();
            if (cn && std::strcmp(cn, name) == 0) return c;
        }
    } catch (...) {}
    return nullptr;
}

// Create a child node with `name` under `parent`. Optional data type
// (DATA_BYTEBLOCK = 2 for text). Returns the new node or nullptr.
treenode createChild(treenode parent, const char* name, int dataType)
{
    if (!parent || !name) return nullptr;
    try {
        treenode n = nodeinsertinto(parent);
        if (!n) return nullptr;
        setname(n, name);
        if (dataType > 0) nodeadddata(n, dataType);
        return n;
    } catch (...) {
        return nullptr;
    }
}

// Find or create a child container (no data type — just a holder).
treenode findOrCreateContainer(treenode parent, const char* name)
{
    treenode c = findChild(parent, name);
    if (c) return c;
    return createChild(parent, name, 0);
}

// Walk to the projectSettings byteblock, creating intermediates as
// needed. We can't always rely on the in-model object being shaped
// the way the library template expects — legacy models cloned from
// an older library lack the variables/projectSettings node. Build
// what's missing so the user's first project-scope save sticks.
treenode ensureProjectSettingsNode()
{
    treenode obj = findInModelObject();
    if (!obj) return nullptr;
    treenode vars = findOrCreateContainer(obj, "variables");
    if (!vars) return nullptr;
    treenode ps = findChild(vars, "projectSettings");
    if (ps) return ps;
    return createChild(vars, "projectSettings", DATA_BYTEBLOCK);
}

// Read-only lookup. Doesn't create the node — readers tolerate a
// missing node by returning an empty object.
treenode findProjectSettingsNodeReadonly()
{
    treenode obj = findInModelObject();
    if (!obj) return nullptr;
    try {
        return obj->find("variables/projectSettings");
    } catch (...) {
        return nullptr;
    }
}

} // namespace

bool projectStorageAvailable()
{
    // The in-model object existing is the gate — once it exists, we
    // can always create the byteblock on demand.
    return findInModelObject() != nullptr;
}

nlohmann::json loadProjectSettings()
{
    treenode n = findProjectSettingsNodeReadonly();
    if (!n) return nlohmann::json::object();
    std::string body;
    try {
        body = gets(n);
    } catch (...) {
        return nlohmann::json::object();
    }
    if (body.empty()) return nlohmann::json::object();
    try {
        auto j = nlohmann::json::parse(body);
        return j.is_object() ? j : nlohmann::json::object();
    } catch (...) {
        bridge::consolePrint("[ModelerAI] WARN: projectSettings parse failed; ignoring\n");
        return nlohmann::json::object();
    }
}

bool saveProjectSettings(const nlohmann::json& j)
{
    treenode n = ensureProjectSettingsNode();
    if (!n) {
        bridge::consolePrint("[ModelerAI] saveProjectSettings: no in-model node (addmodelerai not run yet?)\n");
        return false;
    }
    try {
        sets(n, j.dump());
        return true;
    } catch (...) {
        bridge::consolePrint("[ModelerAI] saveProjectSettings: SDK write failed\n");
        return false;
    }
}

bool patchProjectSettings(const nlohmann::json& patch)
{
    if (!patch.is_object()) return false;
    auto current = loadProjectSettings();
    for (auto it = patch.begin(); it != patch.end(); ++it) {
        current[it.key()] = it.value();
    }
    return saveProjectSettings(current);
}

} // namespace ModelerAi::settings
