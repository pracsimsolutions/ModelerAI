# provider/ — AI provider abstraction + adapters

The agent loop only talks to `IProvider*`. Each LLM (Anthropic, OpenAI, Gemini) has its own adapter implementing the same interface. Swapping providers at runtime is a single pointer swap. Adding a fourth provider later = one new adapter file + one entry in the provider registry.

## Files (planned)

- `provider.h` — `IProvider` abstract interface, `ModelInfo`, `StreamChunk`, `ProviderToolCall`, `TurnHandle` types
- `http_client.cpp / .h` — WinHTTP wrapper. Handles TLS, retries, chunked transfer.
- `sse_parser.cpp / .h` — Server-Sent Events streaming parser. Each provider's SSE format is slightly different (Anthropic's `event:` lines, OpenAI's `[DONE]` sentinel, Gemini's array chunks) — adapters call the generic parser then translate events.
- `anthropic.cpp / .h` — Claude adapter. THIS is the reference implementation; build it first.
- `openai.cpp / .h` — empty shell. Stays unimplemented until Claude flow is solid.
- `gemini.cpp / .h` — empty shell. Same deal.

## The IProvider contract

```cpp
class IProvider {
public:
    virtual ~IProvider() = default;
    virtual std::string  id() const = 0;            // "anthropic"
    virtual std::string  display_name() const = 0;
    virtual std::vector<ModelInfo> models() const = 0;
    virtual bool has_api_key() const = 0;
    virtual TurnHandle stream_turn(
        const Conversation&         conv,
        const std::vector<ToolDef>& tools,
        StreamCallback              onChunk,
        ToolCallCallback            onToolCall,
        DoneCallback                onDone,
        ErrorCallback               onError) = 0;
};
```

## Tool-format translation

The agent gives every adapter the SAME `std::vector<ToolDef>`. Each adapter translates internally:

- Anthropic: `<tool_use>` blocks with JSON-schema-like params
- OpenAI: `tools: [{type:"function", function:{name, parameters}}]`
- Gemini: `parts: [{functionDeclarations: [...]}]`

The agent never sees provider-specific shapes. Tool results go back through the same translation in reverse.

## API endpoints (reference, not committed to)

| Provider | Endpoint | Auth | Streaming |
|---|---|---|---|
| Anthropic | `https://api.anthropic.com/v1/messages` | `x-api-key` header | SSE with `event:` lines |
| OpenAI | `https://api.openai.com/v1/chat/completions` | `Authorization: Bearer ...` | SSE with `[DONE]` sentinel |
| Gemini | `https://generativelanguage.googleapis.com/v1beta/models/<model>:streamGenerateContent` | API key in query | SSE-ish, JSON array chunks |

Per-model lists and version-pinning policy live in the KB topic `KNOWLEDGE/topics/provider-models.md` (not yet written).
