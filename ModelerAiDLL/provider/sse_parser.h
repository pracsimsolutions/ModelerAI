// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/sse_parser.h — incremental Server-Sent Events parser.
//
// Used by the Anthropic adapter (and any future provider that speaks
// SSE) to turn raw HTTP byte chunks into discrete event records.
// Stateful — feed() can be called any number of times with any-sized
// chunks; the parser buffers partial events across calls.

#pragma once

#include <functional>
#include <string>
#include <string_view>

namespace ModelerAi::provider {

struct SseEvent {
    std::string name;   // value of the most recent "event:" line, may be empty
    std::string data;   // concatenation of all "data:" lines for this event,
                        // joined with '\n' per the SSE spec
};

class SseParser {
public:
    using EventCallback = std::function<void(const SseEvent&)>;

    explicit SseParser(EventCallback cb);

    // Feed a raw byte chunk. Any complete event(s) inside the chunk
    // trigger the callback synchronously. A partial event at the end
    // of the chunk is buffered until the next feed() / flush().
    void feed(std::string_view chunk);

    // Emit any pending event. Anthropic always terminates events with
    // a blank line, so this rarely matters in practice, but it's there
    // for defensive use at stream-close time.
    void flush();

    // Drop all buffered state (line buffer, in-progress event) WITHOUT
    // emitting. Used by the Anthropic adapter when retrying after a 429
    // so stale half-parsed bytes from the failed attempt don't leak into
    // the next stream. The event callback is preserved.
    void reset();

private:
    EventCallback onEvent_;
    std::string   lineBuffer_;     // accumulates bytes of the current line
    std::string   currentEvent_;
    std::string   currentData_;
    bool          haveCurrentEvent_ = false; // true if any line of current event seen

    void handleLine_(std::string_view line);
    void emit_();
    static std::string_view trimLeadingSpace_(std::string_view s);
};

} // namespace ModelerAi::provider
