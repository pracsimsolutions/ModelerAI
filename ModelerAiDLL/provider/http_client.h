// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/http_client.h — streaming HTTPS client built on WinHTTP.
//
// RAII handle wrapper, 5/5/10/30 timeouts (longer receive because we
// stream), TLS 1.2 default.

#pragma once

#include <atomic>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace ModelerAi::provider {

struct HttpHeader {
    std::string name;
    std::string value;
};

// When the caller wants specific response headers back (e.g. retry-after,
// anthropic-ratelimit-*), they supply this struct. responseHeaders is
// filled in with the values for every name listed in wantHeaders that
// the server returned. Header NAMES are matched case-insensitively;
// the result map uses the lowercased form as key.
struct ResponseHeaderCapture {
    std::vector<std::string>                     wantHeaders;
    std::unordered_map<std::string, std::string> responseHeaders;
};

// Streaming POST. host is the bare hostname ("api.anthropic.com"), path is
// the full request path including leading slash. headers must NOT include
// Content-Type or Content-Length — both are set internally. HTTPS only.
//
// `onChunk` is called synchronously from the calling thread for every byte
// chunk read off the wire. NOT SSE-parsed — feed it to SseParser.
//
// `cancelFlag` is checked between reads. If non-null and set to true, the
// read loop exits early and the function returns -1 (distinct from 0 =
// network failure and any 2xx-5xx = HTTP-level outcome). Caller owns the
// atomic; lifetime must outlast this call.
//
// Returns:
//   - HTTP status code on success (any 2xx-5xx),
//   - 0 if the request failed at the network layer (DNS, TLS, timeout,
//     handle creation, etc.),
//   - -1 if the cancelFlag tripped mid-stream.
int streamPost(std::string_view host,
               std::string_view path,
               const std::vector<HttpHeader>& headers,
               std::string_view body,
               const std::function<void(std::string_view)>& onChunk,
               const std::atomic<bool>* cancelFlag = nullptr,
               ResponseHeaderCapture* capture = nullptr);

// Same as above but accepts a full URL like "http://localhost:11434/v1/chat/completions".
// Supports http:// (port 80 default) and https:// (port 443 default), and
// explicit "host:port" syntax. Used by the generic OpenAI-compatible
// adapter, which needs to reach arbitrary cloud + local endpoints.
int streamPostUrl(std::string_view url,
                  const std::vector<HttpHeader>& headers,
                  std::string_view body,
                  const std::function<void(std::string_view)>& onChunk,
                  const std::atomic<bool>* cancelFlag = nullptr,
                  ResponseHeaderCapture* capture = nullptr);

} // namespace ModelerAi::provider
