// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "mainthread/mainthread.h"
#include "bridge/log.h"
#include "bridge/diag.h"

#include <windows.h>     // GetCurrentThreadId
#include <atomic>
#include <chrono>
#include <climits>
#include <deque>
#include <mutex>
#include <string>

namespace ModelerAi::mainthread {

namespace {

std::mutex                          g_queueMutex;
std::deque<std::function<void()>>   g_queue;

// Captured by initMainThread() — atomic for the read-by-other-threads /
// write-once-by-main-thread pattern. Zero means "not initialized yet";
// in that case isOnMainThread() returns false (defensive — we'd rather
// enqueue and block briefly than skip the marshal and crash).
std::atomic<DWORD> g_mainThreadId{ 0 };

// Diagnostic: bumped every drainQueue() call (even when the queue is
// empty), so /remote diag can show "last bridgePoll was Xms ago" and
// confirm whether the JS heartbeat is alive.
std::atomic<long long> g_lastDrainMs{ 0 };

long long nowSteadyMs()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()).count();
}

} // namespace

namespace detail {

void enqueue(std::function<void()> task)
{
    std::lock_guard<std::mutex> lk(g_queueMutex);
    g_queue.push_back(std::move(task));
}

bool isOnMainThread()
{
    DWORD captured = g_mainThreadId.load();
    if (captured == 0) return false;
    return GetCurrentThreadId() == captured;
}

} // namespace detail

void initMainThread()
{
    DWORD tid = GetCurrentThreadId();
    g_mainThreadId.store(tid);
    bridge::consolePrint(
        std::string("[ModelerAI] mainthread::init captured tid=")
        + std::to_string(tid) + "\n");
}

void noteDrainCalled()
{
    g_lastDrainMs.store(nowSteadyMs());
}

long long msSinceLastDrain()
{
    long long last = g_lastDrainMs.load();
    if (last == 0) return LLONG_MAX;
    return nowSteadyMs() - last;
}

void drainQueue()
{
    noteDrainCalled();   // even on empty drains — proves bridgePoll is firing

    // Swap the queue out under the lock; run tasks outside the lock.
    // This way a task that itself enqueues more (rare but possible —
    // e.g. a callback that schedules follow-up work) doesn't deadlock,
    // and a worker enqueueing during drain doesn't have to wait on us.
    std::deque<std::function<void()>> snapshot;
    {
        std::lock_guard<std::mutex> lk(g_queueMutex);
        if (g_queue.empty()) return;
        snapshot.swap(g_queue);
    }

    diag::trace("drainQueue: running " + std::to_string(snapshot.size())
                + " task(s) on tid=" + std::to_string(GetCurrentThreadId()));

    for (auto& task : snapshot) {
        try {
            task();
        } catch (const std::exception& e) {
            diag::info(std::string("mainthread task threw std::exception: ") + e.what());
        } catch (...) {
            diag::info("mainthread task threw non-std exception");
        }
    }
    diag::trace("drainQueue: complete");
}

int pendingTaskCount()
{
    std::lock_guard<std::mutex> lk(g_queueMutex);
    return (int)g_queue.size();
}

unsigned long capturedMainThreadId()
{
    return (unsigned long)g_mainThreadId.load();
}

} // namespace ModelerAi::mainthread
