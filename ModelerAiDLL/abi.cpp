// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "abi.h"
#include "bridge/bridge.h"
#include "bridge/log.h"
#include "mainthread/mainthread.h"

#include <exception>
#include <string>

// FlexScript-callable DLL exports.
//
// Pattern lifted from AStarDLL/AStarNavigator.cpp — plain C++ functions
// (NOT extern "C") with __declspec(dllexport), called via function_s(node,
// "name", args) from FlexScript. FlexSim's binding layer locates them by
// their mangled name; an extern "C" wrap broke the call previously.
//
// Each export wraps its body in a try-catch so an unexpected std::exception
// (e.g. from nlohmann::json on malformed input) becomes a tidy error
// envelope instead of an unhandled C++ throw that FlexSim surfaces as
// "Exception caught in TreeNode::callMemberFunction() c++/dll execution"
// in the console — which then fires on every poll tick, infinite-loop style.

// Thread-local buffers — Variants returning const char* must point at
// memory that lives past the return.
static thread_local std::string g_sendBuffer;
static thread_local std::string g_pollBuffer;

modelerai_export Variant ModelerAi_bridgeSend(FLEXSIMINTERFACE)
{
    try {
        std::string envelope = param(1).c_str();
        ModelerAi::bridge::consolePrint("[ModelerAI] ModelerAi_bridgeSend invoked\n");
        if (envelope.empty()) {
            ModelerAi::bridge::consolePrint("[ModelerAI] ERR: empty envelope\n");
            g_sendBuffer = "err:no_envelope";
            return Variant(g_sendBuffer.c_str());
        }
        g_sendBuffer = ModelerAi::bridge::handleEnvelope(envelope);
        return Variant(g_sendBuffer.c_str());
    }
    catch (const std::exception& e) {
        ModelerAi::bridge::consolePrint(std::string("[ModelerAI] EXC bridgeSend: ") + e.what() + "\n");
        g_sendBuffer = std::string("err:exception:") + e.what();
        return Variant(g_sendBuffer.c_str());
    }
    catch (...) {
        ModelerAi::bridge::consolePrint("[ModelerAI] EXC bridgeSend: unknown\n");
        g_sendBuffer = "err:exception:unknown";
        return Variant(g_sendBuffer.c_str());
    }
}

modelerai_export Variant ModelerAi_bridgePoll(FLEXSIMINTERFACE)
{
    try {
        // Drain main-thread tasks FIRST so any worker waiting on a runAndWait
        // gets its result before this tick returns. Then check for FlexSim
        // model-context changes (open / close / Save-As) and trigger the
        // re-anchor flow if needed. Finally collect any outbound bridge
        // events the tasks (or the re-anchor) produced.
        ModelerAi::mainthread::drainQueue();
        ModelerAi::bridge::onPollTick();
        g_pollBuffer = ModelerAi::bridge::drainEvents();
        return Variant(g_pollBuffer.c_str());
    }
    catch (const std::exception& e) {
        ModelerAi::bridge::consolePrint(std::string("[ModelerAI] EXC bridgePoll: ") + e.what() + "\n");
        g_pollBuffer = "[]";
        return Variant(g_pollBuffer.c_str());
    }
    catch (...) {
        ModelerAi::bridge::consolePrint("[ModelerAI] EXC bridgePoll: unknown\n");
        g_pollBuffer = "[]";
        return Variant(g_pollBuffer.c_str());
    }
}
