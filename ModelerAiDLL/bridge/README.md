# bridge/ — JS ↔ C++ message routing

JSON envelopes pass between the HTML viewer (CEF panel) and this DLL. The viewer never knows which provider is in use, what the system prompt says, or what tools exist — it only emits intents and renders chunks it receives back.

## Envelope shape

Canonical form (compact, no whitespace, UTF-8):

```json
{"t":"user_message","id":"01HZ...","p":{"text":"build me a 3-station line"}}
```

- `t` — message type (string, see protocol.h)
- `id` — correlation ID (ULID-like string)
- `p` — payload object, type-specific (may be omitted for some types)

## Types

See [`protocol.h`](protocol.h) for the canonical enum and per-type payload schemas.

**JS → DLL:** `user_message`, `cancel_turn`, `provider_switch`, `save_api_key`, `forget_api_key`, `load_history`, `clear_history`, `get_provider_info`

**DLL → JS:** `assistant_chunk`, `assistant_done`, `tool_call_start`, `tool_call_done`, `state_update`, `error`

## Debug logging

When the env var `MODELER_AI_BRIDGE_LOG=1` is set, every envelope in both directions is appended to `%APPDATA%/PRACSIM/ModelerAI/bridge.log`, pretty-printed with timestamps. No-op in production builds.

## Files (planned)

- `protocol.h` — message-type enum + per-type payload schemas
- `bridge.cpp / bridge.h` — envelope dispatch, registers handlers per type
- `log.cpp / log.h` — `MODELER_AI_BRIDGE_LOG`-driven trace writer
- `json.h` — thin wrapper around `../third_party/json.h`
