// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// keystore/keystore.h — BYOK API key storage, DPAPI-encrypted at rest.

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace ModelerAi::keystore {

class Keystore {
public:
    /** Load (or initialize empty) keys.dat from the given app-data directory.
     *  Returns false on hard I/O errors; an empty/missing file is success. */
    bool load(const std::string& appDataDir);

    /** Persist the current in-memory map back to disk. */
    bool save() const;

    bool has(std::string_view providerId) const;

    /** Decrypt and return the key for a provider. The plaintext lives in
     *  outKey for the caller's scope only; do not log, do not echo to JS. */
    bool get(std::string_view providerId, std::string& outKey) const;

    /** Store / replace a key. Persists on the next save(). */
    void set(std::string_view providerId, std::string_view key);

    /** Remove a key. Persists on the next save(). */
    void forget(std::string_view providerId);

private:
    std::string                                  appDataDir_;
    // providerId -> DPAPI-encrypted blob (opaque bytes as string)
    std::unordered_map<std::string, std::string> blobs_;
};

} // namespace ModelerAi::keystore
