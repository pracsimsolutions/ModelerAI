// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// remote/asset_loader.h — loads + caches the viewer HTML for the
// remote HTTP server to serve.

#pragma once

#include <string>

namespace ModelerAi::remote {

// Returns the contents of <install dir>/webview/index.html. Reads from
// disk on first call, caches in memory thereafter. Returns an empty
// string if the file is missing.
const std::string& loadEmbeddedViewer();

// Forces a reload from disk on next loadEmbeddedViewer() call. Useful
// during webview development; not surfaced as a tool.
void invalidateViewerCache();

} // namespace ModelerAi::remote
