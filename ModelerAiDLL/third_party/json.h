// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// third_party/json.h - Defensive wrapper around nlohmann/json single-header.
//
// Why this exists: Windows SDK headers (specifically rpcndr.h, included
// transitively by windows.h, and by FlexSim's SDK) `#define small char`,
// which collides with nlohmann/json's internal use of `small` as an
// identifier name and breaks the parse with cryptic syntax errors deep
// in json.hpp (around lines 18000+). When any .cpp pulls in a header
// that includes <windows.h> before this wrapper, those macros pollute
// the preprocessor state and json.hpp fails to compile.
//
// The fix is to #undef the offenders right before including json.hpp.
// We also drop a few other macros that commonly bite C++ libraries on
// Windows: IN / OUT / OPTIONAL (rpcndr.h, oaidl.h).
//
// Every ModelerAI .cpp that needs nlohmann/json must include THIS header
// instead of "thirdparty/nlohmann/json.hpp" directly — the wrapper handles
// FlexSim's `small` / `min` / `max` macro pollution before pulling in the
// upstream single-header.

#pragma once

#ifdef small
#undef small
#endif
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif
#ifdef OPTIONAL
#undef OPTIONAL
#endif

// FlexSim SDK macros that collide with json.hpp identifiers.
// BasicMacros.h:162  `#define o(type,instance)` clobbers serializer's `o(...)`
// initializer (json.hpp:18077). Undef before json includes; FlexSim code that
// actually needs the `o(T, n)` cast macro re-uses it after — the def is in the
// SDK header so it comes right back the next time something includes module.h.
#ifdef o
#undef o
#endif

#include "thirdparty/nlohmann/json.hpp"
