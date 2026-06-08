// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// DPAPI-encrypted at-rest storage for BYOK API keys.
//
// Each provider's plaintext is encrypted via CryptProtectData with the
// current user's logon credentials as the entropy. Resulting blob is
// stored in keys.dat (a small JSON map of providerId -> base64-encoded
// blob). Only the same Windows user on the same machine can decrypt —
// roaming the file to another machine yields garbage.

#include "keystore/keystore.h"
#include "bridge/log.h"
#include "third_party/json.h"

#include <windows.h>
#include <dpapi.h>     // CryptProtectData / CryptUnprotectData
#include <wincrypt.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "crypt32.lib")

namespace ModelerAi::keystore {

namespace {

constexpr const char* kFileName = "keys.dat";

std::string base64Encode(const unsigned char* data, size_t len)
{
    if (len == 0) return "";
    DWORD outLen = 0;
    CryptBinaryToStringA(data, (DWORD)len,
        CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &outLen);
    if (outLen == 0) return "";
    std::string out(outLen, '\0');
    if (!CryptBinaryToStringA(data, (DWORD)len,
        CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
        out.data(), &outLen)) return "";
    out.resize(outLen);
    return out;
}

bool base64Decode(const std::string& s, std::vector<unsigned char>& out)
{
    if (s.empty()) { out.clear(); return true; }
    DWORD len = 0;
    if (!CryptStringToBinaryA(s.c_str(), (DWORD)s.size(),
        CRYPT_STRING_BASE64, nullptr, &len, nullptr, nullptr)) return false;
    out.assign(len, 0);
    return CryptStringToBinaryA(s.c_str(), (DWORD)s.size(),
        CRYPT_STRING_BASE64, out.data(), &len, nullptr, nullptr) != 0;
}

// Encrypt plaintext with DPAPI. Tied to the current Windows user account.
bool dpapiEncrypt(std::string_view plaintext, std::vector<unsigned char>& out)
{
    DATA_BLOB in{};
    in.pbData = (BYTE*)plaintext.data();
    in.cbData = (DWORD)plaintext.size();
    DATA_BLOB enc{};
    if (!CryptProtectData(&in, L"ModelerAI",
                          /*pOptionalEntropy*/ nullptr,
                          nullptr, nullptr, 0, &enc))
    {
        return false;
    }
    out.assign(enc.pbData, enc.pbData + enc.cbData);
    LocalFree(enc.pbData);
    return true;
}

bool dpapiDecrypt(const std::vector<unsigned char>& blob, std::string& outPlain)
{
    if (blob.empty()) { outPlain.clear(); return true; }
    DATA_BLOB in{};
    in.pbData = (BYTE*)blob.data();
    in.cbData = (DWORD)blob.size();
    DATA_BLOB plain{};
    LPWSTR descOut = nullptr;
    if (!CryptUnprotectData(&in, &descOut,
                            nullptr, nullptr, nullptr, 0, &plain))
    {
        return false;
    }
    outPlain.assign((const char*)plain.pbData, plain.cbData);
    if (descOut) LocalFree(descOut);
    LocalFree(plain.pbData);
    return true;
}

std::string filePath(const std::string& dir)
{
    if (dir.empty()) return "";
    return dir + kFileName;
}

} // namespace

bool Keystore::load(const std::string& appDataDir)
{
    appDataDir_ = appDataDir;
    blobs_.clear();
    std::string path = filePath(appDataDir_);
    if (path.empty()) return false;

    std::ifstream f(path, std::ios::binary);
    if (!f) {
        // Missing is success — empty keystore.
        return true;
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    std::string body = ss.str();
    if (body.empty()) return true;

    try {
        auto j = nlohmann::json::parse(body);
        if (j.contains("keys") && j["keys"].is_object()) {
            for (auto it = j["keys"].begin(); it != j["keys"].end(); ++it) {
                if (!it.value().is_string()) continue;
                blobs_[it.key()] = it.value().get<std::string>();
            }
        }
        bridge::consolePrint(("[ModelerAI] keystore loaded entries=" +
                              std::to_string(blobs_.size()) + "\n"));
        return true;
    } catch (...) {
        bridge::consolePrint("[ModelerAI] WARN: keystore parse failed; starting fresh\n");
        return false;
    }
}

bool Keystore::save() const
{
    std::string path = filePath(appDataDir_);
    if (path.empty()) return false;

    nlohmann::json j;
    j["version"] = 1;
    nlohmann::json kmap = nlohmann::json::object();
    for (const auto& kv : blobs_) {
        kmap[kv.first] = kv.second;
    }
    j["keys"] = std::move(kmap);

    std::string tmp = path + ".tmp";
    {
        std::ofstream f(tmp, std::ios::binary);
        if (!f) return false;
        f << j.dump(2);
    }
    return MoveFileExA(tmp.c_str(), path.c_str(),
                      MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
}

bool Keystore::has(std::string_view providerId) const
{
    return blobs_.find(std::string(providerId)) != blobs_.end();
}

bool Keystore::get(std::string_view providerId, std::string& outKey) const
{
    auto it = blobs_.find(std::string(providerId));
    if (it == blobs_.end()) return false;
    std::vector<unsigned char> blob;
    if (!base64Decode(it->second, blob)) return false;
    return dpapiDecrypt(blob, outKey);
}

void Keystore::set(std::string_view providerId, std::string_view key)
{
    std::vector<unsigned char> blob;
    if (!dpapiEncrypt(key, blob)) {
        bridge::consolePrint("[ModelerAI] ERR: DPAPI encrypt failed\n");
        return;
    }
    blobs_[std::string(providerId)] = base64Encode(blob.data(), blob.size());
}

void Keystore::forget(std::string_view providerId)
{
    blobs_.erase(std::string(providerId));
}

} // namespace ModelerAi::keystore
