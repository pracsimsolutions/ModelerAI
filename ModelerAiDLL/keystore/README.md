# keystore/ — BYOK API key storage

User-supplied API keys are encrypted at rest using Windows DPAPI and stored at `%APPDATA%/PRACSIM/ModelerAI/keys.dat`. JS in the viewer can request that a key be saved (`save_api_key` bridge envelope) and the DLL re-encrypts; JS NEVER sees the plaintext after that point. Keys are decrypted in-memory only at the moment of an HTTP request to the provider.

## Files (planned)

- `keystore.cpp / .h` — load/save/encrypt/decrypt API; one entry per provider id
- `keystore.test.cpp` — round-trip tests (local-build only, not in production builds)

## DPAPI scope

`CryptProtectData` with `CRYPTPROTECT_LOCAL_MACHINE` flag → the encrypted blob can be decrypted by any user on the same machine. Per-user scope (no flag) would force re-entry of the key for every Windows user account, which is hostile. Machine scope matches the threat model: the user keeping FlexSim on a personal/work machine is fine with another user on the same box reading their key (Windows account separation is the trust boundary upstream).

If/when threat-model demands per-user scope, flip the flag and add an "encrypted-by" entry per blob. Out of v1 scope.

## Format on disk

```
%APPDATA%/PRACSIM/ModelerAI/keys.dat
  → opaque binary; one DPAPI-encrypted blob per provider id, concatenated
    with a small header. See keystore.cpp for the layout.
```

JSON would be readable to anyone with file access; the binary blob is
opaque enough that casual inspection won't reveal keys even before DPAPI
encryption kicks in.

## Methods (sketch)

```cpp
class Keystore {
public:
    bool load(const std::string& appDataDir);
    bool save() const;
    bool has(std::string_view providerId) const;
    bool get(std::string_view providerId, std::string& outKey) const;  // returns plaintext
    void set(std::string_view providerId, std::string_view key);
    void forget(std::string_view providerId);
};
```

The provider adapter holds a const reference to the Keystore and calls `get()` only at the moment of building an HTTP request. The plaintext doesn't persist anywhere outside that local scope.
