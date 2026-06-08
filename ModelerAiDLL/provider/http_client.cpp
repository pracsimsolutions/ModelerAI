// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/http_client.h"

#include <windows.h>
#include <winhttp.h>
#include <atomic>
#include <string>

#pragma comment(lib, "winhttp.lib")

namespace ModelerAi::provider {

namespace {

// RAII WinHTTP handle. Closes every handle on early-return paths without
// manual branching.
struct WinHttpHandle {
    HINTERNET h = nullptr;
    WinHttpHandle() = default;
    explicit WinHttpHandle(HINTERNET handle) : h(handle) {}
    ~WinHttpHandle() { if (h) WinHttpCloseHandle(h); }

    WinHttpHandle(const WinHttpHandle&) = delete;
    WinHttpHandle& operator=(const WinHttpHandle&) = delete;
    WinHttpHandle(WinHttpHandle&& other) noexcept : h(other.h) { other.h = nullptr; }
    WinHttpHandle& operator=(WinHttpHandle&& other) noexcept {
        if (this != &other) { if (h) WinHttpCloseHandle(h); h = other.h; other.h = nullptr; }
        return *this;
    }

    explicit operator bool() const { return h != nullptr; }
    HINTERNET get() const { return h; }
};

std::wstring toWide(std::string_view s)
{
    if (s.empty()) return L"";
    int len = MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), nullptr, 0);
    std::wstring ws(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, s.data(), (int)s.size(), &ws[0], len);
    return ws;
}

struct ParsedUrl {
    std::string host;
    std::string path;     // includes query string if any
    INTERNET_PORT port;
    bool         secure;
    bool         valid = false;
};

// Minimal URL parser — sufficient for "http(s)://host[:port]/path".
// Doesn't bother with auth-in-URL or fragments; LLM endpoints don't use them.
ParsedUrl parseUrl(std::string_view url)
{
    ParsedUrl r;
    std::string u(url);
    if (u.compare(0, 8, "https://") == 0) {
        r.secure = true;
        r.port   = INTERNET_DEFAULT_HTTPS_PORT;
        u.erase(0, 8);
    } else if (u.compare(0, 7, "http://") == 0) {
        r.secure = false;
        r.port   = INTERNET_DEFAULT_HTTP_PORT;
        u.erase(0, 7);
    } else {
        return r;  // invalid — must be a recognized scheme
    }
    auto slash = u.find('/');
    std::string hostPort = (slash == std::string::npos) ? u : u.substr(0, slash);
    r.path = (slash == std::string::npos) ? std::string("/") : u.substr(slash);
    if (r.path.empty()) r.path = "/";

    auto colon = hostPort.find(':');
    if (colon == std::string::npos) {
        r.host = hostPort;
    } else {
        r.host = hostPort.substr(0, colon);
        try { r.port = (INTERNET_PORT)std::stoi(hostPort.substr(colon + 1)); }
        catch (...) { return r; }
    }
    if (r.host.empty()) return r;
    r.valid = true;
    return r;
}

// Lower-cases a string in place. ASCII is enough for HTTP header names.
inline std::string asciiLower(std::string_view s) {
    std::string out(s);
    for (auto& c : out) {
        if (c >= 'A' && c <= 'Z') c = static_cast<char>(c - 'A' + 'a');
    }
    return out;
}

int doStreamPost(const std::string& host,
                 const std::string& path,
                 INTERNET_PORT port,
                 bool secure,
                 const std::vector<HttpHeader>& headers,
                 std::string_view body,
                 const std::function<void(std::string_view)>& onChunk,
                 const std::atomic<bool>* cancelFlag,
                 ResponseHeaderCapture* capture)
{
    auto cancelled = [&]() { return cancelFlag && cancelFlag->load(); };
    if (cancelled()) return -1;
    WinHttpHandle session(WinHttpOpen(
        L"ModelerAi/1.0",
        WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
        WINHTTP_NO_PROXY_NAME,
        WINHTTP_NO_PROXY_BYPASS,
        0));
    if (!session) return 0;
    WinHttpSetTimeouts(session.get(), 5000, 5000, 10000, 30000);

    std::wstring whost = toWide(host);
    WinHttpHandle connect(WinHttpConnect(session.get(), whost.c_str(), port, 0));
    if (!connect) return 0;

    std::wstring wpath = toWide(path);
    DWORD flags = secure ? WINHTTP_FLAG_SECURE : 0;
    WinHttpHandle request(WinHttpOpenRequest(
        connect.get(), L"POST", wpath.c_str(),
        nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags));
    if (!request) return 0;

    std::wstring hdrBlob = L"Content-Type: application/json";
    for (const auto& h : headers) {
        hdrBlob += L"\r\n";
        hdrBlob += toWide(h.name);
        hdrBlob += L": ";
        hdrBlob += toWide(h.value);
    }
    WinHttpAddRequestHeaders(request.get(), hdrBlob.c_str(), (DWORD)-1,
        WINHTTP_ADDREQ_FLAG_ADD | WINHTTP_ADDREQ_FLAG_REPLACE);

    LPVOID bodyPtr = body.empty() ? WINHTTP_NO_REQUEST_DATA : (LPVOID)body.data();
    DWORD  bodyLen = (DWORD)body.size();
    if (!WinHttpSendRequest(request.get(),
            WINHTTP_NO_ADDITIONAL_HEADERS, 0, bodyPtr, bodyLen, bodyLen, 0))
        return 0;
    if (!WinHttpReceiveResponse(request.get(), nullptr)) return 0;

    DWORD statusCode = 0, statusSize = sizeof(statusCode);
    if (!WinHttpQueryHeaders(request.get(),
            WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
            WINHTTP_HEADER_NAME_BY_INDEX,
            &statusCode, &statusSize, WINHTTP_NO_HEADER_INDEX))
        return 0;

    // Capture any requested response headers (used by the Anthropic adapter
    // for `retry-after` + `anthropic-ratelimit-*` on 429). Header NAMES are
    // case-insensitive per RFC 9110; WinHttpQueryHeaders treats the name as
    // case-sensitive when used with WINHTTP_QUERY_CUSTOM, so we pass exactly
    // what the caller asked for. The result map stores under lowercased keys.
    if (capture) {
        for (const auto& hname : capture->wantHeaders) {
            std::wstring wname = toWide(hname);
            DWORD bufLen = 0;
            // First call: get required buffer length.
            if (!WinHttpQueryHeaders(request.get(),
                    WINHTTP_QUERY_CUSTOM,
                    wname.c_str(),
                    WINHTTP_NO_OUTPUT_BUFFER,
                    &bufLen,
                    WINHTTP_NO_HEADER_INDEX)) {
                DWORD err = GetLastError();
                if (err == ERROR_WINHTTP_HEADER_NOT_FOUND) continue;
                if (err != ERROR_INSUFFICIENT_BUFFER) continue;
            }
            std::wstring wval;
            wval.resize(bufLen / sizeof(wchar_t));
            if (WinHttpQueryHeaders(request.get(),
                    WINHTTP_QUERY_CUSTOM,
                    wname.c_str(),
                    &wval[0],
                    &bufLen,
                    WINHTTP_NO_HEADER_INDEX)) {
                // Trim the trailing null WinHTTP includes in bufLen.
                while (!wval.empty() && wval.back() == L'\0') wval.pop_back();
                // Convert wstring -> utf-8.
                int needed = WideCharToMultiByte(CP_UTF8, 0,
                    wval.c_str(), (int)wval.size(), nullptr, 0, nullptr, nullptr);
                std::string out(needed, 0);
                WideCharToMultiByte(CP_UTF8, 0,
                    wval.c_str(), (int)wval.size(), &out[0], needed, nullptr, nullptr);
                capture->responseHeaders[asciiLower(hname)] = std::move(out);
            }
        }
    }

    char buf[4096];
    DWORD avail = 0;
    while (WinHttpQueryDataAvailable(request.get(), &avail) && avail > 0) {
        if (cancelled()) return -1;
        DWORD toRead = (avail < (DWORD)sizeof(buf)) ? avail : (DWORD)sizeof(buf);
        DWORD read = 0;
        if (!WinHttpReadData(request.get(), buf, toRead, &read) || read == 0) break;
        if (onChunk) onChunk(std::string_view(buf, read));
        if (cancelled()) return -1;
    }
    return (int)statusCode;
}

} // namespace

int streamPost(std::string_view host,
               std::string_view path,
               const std::vector<HttpHeader>& headers,
               std::string_view body,
               const std::function<void(std::string_view)>& onChunk,
               const std::atomic<bool>* cancelFlag,
               ResponseHeaderCapture* capture)
{
    return doStreamPost(std::string(host), std::string(path),
                        INTERNET_DEFAULT_HTTPS_PORT, /*secure*/ true,
                        headers, body, onChunk, cancelFlag, capture);
}

int streamPostUrl(std::string_view url,
                  const std::vector<HttpHeader>& headers,
                  std::string_view body,
                  const std::function<void(std::string_view)>& onChunk,
                  const std::atomic<bool>* cancelFlag,
                  ResponseHeaderCapture* capture)
{
    ParsedUrl p = parseUrl(url);
    if (!p.valid) return 0;
    return doStreamPost(p.host, p.path, p.port, p.secure, headers, body, onChunk, cancelFlag, capture);
}

} // namespace ModelerAi::provider
