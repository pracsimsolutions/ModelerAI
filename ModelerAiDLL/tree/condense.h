// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tree/condense.h — produce a condensed text outline of the active model.
//
// Output format (matches docs/architecture/2026-05-23-v0-prototype-design.md §7):
//
//   model:
//     Source1 (Source) @[5, 5, 0]
//       output[0]→ Queue1
//     Queue1 (Queue) @[10, 5, 0]
//       output[0]→ Processor1
//     ...
//
// Empty model returns the literal string "model: (empty)".
//
// MUST be called on the FlexSim main thread — touches model() and walks the
// live tree. The Anthropic worker thread snapshots the result before
// dispatching the HTTP call.

#pragma once

#include <string>

namespace ModelerAi::tree {

std::string condenseActiveModel();

// Mark the cached model-block as stale. The next call to
// condenseActiveModel() will re-walk the tree. Mutating tools
// (create_object / delete_object / connect_* / disconnect / clone /
// add_parameter / run_script with non-empty body / etc.) must call
// this BEFORE returning so the next turn's SNOWBALL reflects reality.
// Read-only tools (inspect / get_*) can skip it — the cache stays
// valid and the per-turn condense cost drops from O(tree-walk) to
// O(string-copy + 2 SDK calls for sim state).
void invalidateCondenseCache();

} // namespace ModelerAi::tree
