// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"
#include "bootstrap.h"
#include "abi.h"   // forward decl pulled in for the bridge exports
#include "bridge/bridge.h"
#include "third_party/json.h"

visible ObjectDataType* createodtderivative(char* classname)
{
	// No derived ObjectDataTypes — ModelerAI's in-model object uses the
	// stock library ModelerAI class with byteblock storage only.
	return NULL;
}

visible SimpleDataType* createsdtderivative(char* classname)
{
	return NULL;
}

// ============================================================================
// Bridge wiring
//
// JS calls `fireFlexsimEvent("modelerAiSend", envelope, cb)`. That lands
// in a FlexScript event handler under the html view's >variables, which
// does:
//
//     return applicationcommand("modelerAiBridgeSend", envelope);
//
// The .fsx ships a sibling user-command-style node (under add_nodefunctions)
// bound to the ModelerAi_bridgeSend / ModelerAi_bridgePoll exports in
// abi.cpp via `dll:"module:ModelerAI" func:"..."`. The same pattern is
// used for every entry in the modelerai_* command library (see
// commands/ping.cpp + .fsx around the bridge bindings).
// ============================================================================

visible void dllinitialize()
{
	pt("[ModelerAI] dllinitialize: bootstrap::initialize()\n");
	ModelerAi::bootstrap::initialize();
	pt("[ModelerAI] dllinitialize: complete\n");
}

// Temporarily disabled — causing issues on session compile/unload.
// TODO: re-enable after investigating the teardown / unregister sequence.
//visible void dllcleanup()
//{
//	pt("[ModelerAI] dllcleanup\n");
//	ModelerAi::bootstrap::teardown();
//}
