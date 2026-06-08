// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#pragma once

#include "FlexsimDefs.h"
#include "allobjects.h"

// Export macro for module-level DLL functions registered via addcommand().
// COMPILING_MODULE_DLL is set in module.vcxproj for every configuration.
#ifdef COMPILING_MODULE_DLL
#define modelerai_export __declspec(dllexport)
#else
#define modelerai_export __declspec(dllimport)
#endif
