// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// abi.h — declarations for the FlexScript-callable DLL exports.
//
// These functions are registered with FlexSim's command table in
// module.cpp::dllinitialize via addcommand. The viewer calls them
// indirectly through the FlexScript event handlers under the html
// view's >variables (see ModelerAI.fsx).

#pragma once

#include "module.h"

// NOT extern "C" — mirror AStar's pattern. FlexSim's function_s dispatch
// locates these via the FSX `dll:` binding nodes, and the mangled C++ name
// is what gets looked up. extern "C" breaks the binding.

modelerai_export Variant ModelerAi_bridgeSend(FLEXSIMINTERFACE);
modelerai_export Variant ModelerAi_bridgePoll(FLEXSIMINTERFACE);
