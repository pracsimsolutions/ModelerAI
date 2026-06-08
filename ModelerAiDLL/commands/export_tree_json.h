// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/export_tree_json.h — modelerai_export_tree_json forward decl.
//
// Walks a tree subtree (selected node, treenode arg, or string path) and
// emits an annotated JSON dump. Designed for learning-session evidence
// drops: the modeler picks a node, exports it, hands the file to the AI.
// Also AI-callable through modelerai_call with output="inline" for
// in-conversation tree introspection.

#pragma once

#include "module.h"

modelerai_export Variant ModelerAi_ExportTreeJson(FLEXSIMINTERFACE);
