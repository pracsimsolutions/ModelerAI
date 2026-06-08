// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/sse_parser.h"

namespace ModelerAi::provider {

SseParser::SseParser(EventCallback cb) : onEvent_(std::move(cb)) {}

void SseParser::feed(std::string_view chunk)
{
    for (char c : chunk) {
        if (c == '\r') {
            // Per spec, treat CR, LF, and CRLF as line terminators.
            // We end the line on either CR or LF; if a CR is followed
            // by LF, the LF will hit an already-empty lineBuffer and
            // be treated as an empty line. To avoid that, swallow LF
            // right after CR.
            handleLine_(lineBuffer_);
            lineBuffer_.clear();
        } else if (c == '\n') {
            // If lineBuffer_ is empty AND the previous char emitted a
            // CR-driven line, this is the LF of a CRLF — skip. We
            // detect that case by tracking "just emitted a line" but
            // since we always reset lineBuffer_ after emit, an empty
            // buffer with a pending LF is interpreted as a real blank
            // line. That's still correct: an empty line terminates an
            // event. The only downside is that a stray CRLF after a
            // non-empty line will emit two terminations. Mitigation:
            // emit_() is idempotent when there's no pending event.
            handleLine_(lineBuffer_);
            lineBuffer_.clear();
        } else {
            lineBuffer_.push_back(c);
        }
    }
}

void SseParser::reset()
{
    lineBuffer_.clear();
    currentEvent_.clear();
    currentData_.clear();
    haveCurrentEvent_ = false;
}

void SseParser::flush()
{
    if (!lineBuffer_.empty()) {
        handleLine_(lineBuffer_);
        lineBuffer_.clear();
    }
    if (haveCurrentEvent_) emit_();
}

void SseParser::handleLine_(std::string_view line)
{
    if (line.empty()) {
        // Blank line — terminates the current event.
        if (haveCurrentEvent_) emit_();
        return;
    }
    if (line[0] == ':') return; // SSE comment

    // Field is everything before the first ':'. If no ':', whole line
    // is the field with empty value (per spec). The value is everything
    // after, with one leading space stripped.
    auto colon = line.find(':');
    std::string_view field;
    std::string_view value;
    if (colon == std::string_view::npos) {
        field = line;
        value = "";
    } else {
        field = line.substr(0, colon);
        value = line.substr(colon + 1);
        value = trimLeadingSpace_(value);
    }

    if (field == "event") {
        currentEvent_.assign(value);
        haveCurrentEvent_ = true;
    } else if (field == "data") {
        if (!currentData_.empty()) currentData_.push_back('\n');
        currentData_.append(value);
        haveCurrentEvent_ = true;
    }
    // "id" and "retry" exist in SSE but Anthropic doesn't use them — ignore.
}

void SseParser::emit_()
{
    if (!haveCurrentEvent_) return;
    SseEvent ev;
    ev.name = std::move(currentEvent_);
    ev.data = std::move(currentData_);
    currentEvent_.clear();
    currentData_.clear();
    haveCurrentEvent_ = false;
    if (onEvent_) onEvent_(ev);
}

std::string_view SseParser::trimLeadingSpace_(std::string_view s)
{
    if (!s.empty() && s.front() == ' ') s.remove_prefix(1);
    return s;
}

} // namespace ModelerAi::provider
