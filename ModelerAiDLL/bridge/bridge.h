// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bridge/bridge.h — envelope dispatcher + outbound queue.
//
// The bridge sits between three callers:
//   - FlexScript-callable DLL exports in abi.cpp (JS-driven inbound).
//   - The agent / provider layer (DLL-driven outbound).
//   - The bridge log (always observes envelope traffic in both directions).
//
// Outbound envelopes are queued; JS drains via poll() on a setInterval timer
// while a turn is active. This avoids the DLL needing a way to call into JS
// from a background thread (which FlexSim's CEF panel doesn't easily allow).

#pragma once

#include <string>
#include <string_view>

namespace ModelerAi::bridge {

// Called from the FlexScript bridge entry (modelerAiBridgeSend) AND from
// the remote HTTP server (/api/send). Parses the envelope, routes by type,
// returns a short ack ("ok" / "err:<reason>"). The real reply for the
// turn is queued via pushEvent() / pushEventTo() and delivered when the
// requester (or the broadcast set of subscribers) next polls.
//
// `requesting_sid` identifies which subscriber sent this envelope:
//   - kLocalFlexsimSubscriberId (default) for the in-FlexSim CEF viewer
//   - the X-Mraisid / cookie sid for a remote phone/browser
// Some handlers (notably viewer_ready) use it to TARGET their response
// at the requester instead of broadcasting, so a phone connecting
// doesn't stomp the desktop viewer's state — the core fix the audit
// at docs/superpowers/specs/2026-06-05-remote-viewer-audit.md identified.
std::string handleEnvelope(std::string_view envelopeJson,
                            std::string_view requesting_sid = "local-flexsim");

// Drains all queued outbound envelopes. Returns a JSON array string,
// e.g. [{"t":"assistant_chunk",...},{"t":"assistant_done",...}].
// Empty array if nothing pending.
std::string drainEvents();

// Used by the agent + provider to enqueue outbound envelopes for
// broadcast (every subscriber's queue). Thread-safe.
void pushEvent(const std::string& envelopeJson);

// Push to ONE specific subscriber's queue (no fan-out). Used for
// targeted responses — viewer_ready replies, list_sessions responses,
// anything that's a reply-to-a-request rather than agent output.
// No-op if `sid` isn't a registered subscriber. Thread-safe.
void pushEventTo(std::string_view sid, const std::string& envelopeJson);

// Called from bridgePoll on every FlexSim main-thread tick. Detects when
// the active FlexSim model changed since the last poll (a different
// .fsm was opened, or addmodelerai was just run on the current one) and
// triggers the auto-switch flow: cancel any in-flight turn (preserving
// partial reply in the OLD session), apply the Save-As policy if it
// looks like a Save-As branch, then push a fresh `session` envelope so
// the viewer rehydrates with the new model's chat. Cheap when nothing
// has changed.
void onPollTick();

} // namespace ModelerAi::bridge
