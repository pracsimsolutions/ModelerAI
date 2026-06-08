// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// mainthread/mainthread.h — main-thread task queue.
//
// THE PROBLEM: FlexSim's SDK is single-threaded. Only the FlexSim main
// thread is allowed to read/mutate the tree. Our Agent runs its turn
// loop on a worker std::thread (so the LLM stream doesn't block FlexSim's
// UI). Tool bodies need to call SDK functions (Object.create, setLocation,
// etc.). Calling those from the worker would crash or corrupt state.
//
// THE SOLUTION: workers enqueue a closure here, then block on its future.
// The FlexSim main thread drains the queue during the existing
// `ModelerAi_bridgePoll` heartbeat (50ms tick), running each closure
// in turn. Worker wakes up with the result.
//
// USAGE — from a tool body's run() (which fires on the worker thread):
//
//     #include "mainthread/mainthread.h"
//
//     // task returns a value
//     std::string path = ModelerAi::mainthread::runAndWait([&]() {
//         treenode obj = Object::create("Source");
//         setname(obj, "Source1");
//         return std::string(obj.name);
//     });
//
//     // task is void
//     ModelerAi::mainthread::runAndWait([&]() {
//         someNode.destroy();
//     });
//
// If runAndWait is invoked WHILE already on the main thread, it runs
// inline (no enqueue, no block). This makes it safe for nested calls
// and for code paths that might fire from either thread.
//
// EXCEPTIONS: if the closure throws, the exception is re-thrown on the
// worker when it calls future.get(). Wrap tool bodies in try/catch so
// SDK errors become tidy ToolResult error_messages.

#pragma once

#include <functional>
#include <future>
#include <memory>
#include <type_traits>
#include <utility>

namespace ModelerAi::mainthread {

// ----- detail surface (not part of the public API) ------------------
namespace detail {
    // Enqueue a type-erased no-arg closure. Called by runAndWait after it
    // has set up the promise + future pair.
    void enqueue(std::function<void()> task);

    // True iff the calling thread's ID matches the captured main-thread ID.
    // initMainThread() must have been called once first.
    bool isOnMainThread();
} // namespace detail

// Capture the FlexSim main thread ID. Call ONCE from the main thread
// during DLL init (bootstrap::initialize). Subsequent isOnMainThread()
// checks compare against this value.
void initMainThread();

// Drain the task queue. Call from a FlexSim-main-thread heartbeat
// (currently ModelerAi_bridgePoll). Runs each pending task to completion;
// safe to call when the queue is empty. Exceptions from individual tasks
// are caught + logged so one bad task doesn't break the drain pass.
void drainQueue();

// Diagnostic-only: current depth of the main-thread queue.
int pendingTaskCount();

// Diagnostic-only: thread id captured by initMainThread() (zero if not
// yet initialized). Used by `/remote diag` to confirm the right thread
// is draining.
unsigned long capturedMainThreadId();

// Diagnostic-only: ms since the last drainQueue() call. INT_MAX if it
// has never been called. Lets `/remote diag` confirm whether the JS
// heartbeat is keeping the main-thread queue serviced.
long long msSinceLastDrain();

// Diagnostic-only: increment a counter every time drainQueue() is called
// (whether or not it had work). Use msSinceLastDrain() to inspect.
void noteDrainCalled();

// Run `task` on the FlexSim main thread, block until it completes, return
// its result. Thread-safe; safe from any worker thread. If invoked while
// already on the main thread, runs inline (no enqueue, no block) so
// nested calls and dual-thread call sites both work.
//
// Throws on closure exception: whatever the closure throws is re-thrown
// from this call after the future resolves.
template <typename Fn>
auto runAndWait(Fn&& task) -> std::invoke_result_t<std::decay_t<Fn>>
{
    using R = std::invoke_result_t<std::decay_t<Fn>>;

    // Inline path — calling from the main thread already.
    if (detail::isOnMainThread()) {
        return task();
    }

    // Promise/future pair. shared_ptr because we hand the promise into a
    // std::function (which requires copyability of the captured state).
    auto promise = std::make_shared<std::promise<R>>();
    auto future = promise->get_future();

    detail::enqueue([promise, taskCopy = std::forward<Fn>(task)]() mutable {
        try {
            if constexpr (std::is_void_v<R>) {
                taskCopy();
                promise->set_value();
            } else {
                promise->set_value(taskCopy());
            }
        } catch (...) {
            promise->set_exception(std::current_exception());
        }
    });

    if constexpr (std::is_void_v<R>) {
        future.get();
        return;
    } else {
        return future.get();
    }
}

} // namespace ModelerAi::mainthread
