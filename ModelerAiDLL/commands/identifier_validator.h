// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/identifier_validator.h — pre-execution FlexScript identifier
// validator.
//
// The goal: stop the AI from hallucinating command names that don't
// exist in FlexSim. The agent prompt tells the model what's real, but
// as context grows, the prompt loses to general coding muscle memory
// (createObject, createInstance, makeObject, etc. — none of which
// exist in the FlexSim catalog).
//
// Strategy: at module init, build a set of every known identifier from
// the loaded KB (~790 active commands + ~1216 deprecated + ~116 class
// names + modelerai_* tools). Before run_script hands a script to
// executestring, parse the script for top-level identifier(...) call
// shapes that AREN'T method calls (not preceded by `.` or `::`), and
// reject the script if any identifier isn't in the set. The error
// response embeds KB search suggestions per unknown name so the AI's
// next turn sees the real catalog.
//
// Method calls (`obj.foo()`, `ClassName::bar()`) aren't validated —
// the static set can't typecheck instance method dispatch, and trying
// to do so would generate noisy false positives. The validator's
// scope is the global function namespace, which is exactly where the
// hallucinations land.

#pragma once

#include <string>
#include <vector>

namespace ModelerAi {

struct ValidationFinding {
    enum Status { Unknown, Deprecated };
    std::string name;
    Status      status = Unknown;
    std::string replacedBy;    // populated when Status == Deprecated
    std::string didYouMean;    // populated when Status == Unknown AND a high-confidence
                               // normalization (case / snake↔camel) lands on a known name
    int         approx_line = 0; // 1-based, in the validated script
};

struct ValidationResult {
    std::vector<ValidationFinding> findings;

    bool hasUnknown() const {
        for (const auto& f : findings) {
            if (f.status == ValidationFinding::Unknown) return true;
        }
        return false;
    }
    bool hasDeprecated() const {
        for (const auto& f : findings) {
            if (f.status == ValidationFinding::Deprecated) return true;
        }
        return false;
    }
};

// Rebuilds the in-process identifier catalog from the loaded KB.
// Idempotent. Safe to call at any time after kbIndex().load() returns
// true; if called before, the catalog stays empty and validation
// becomes a no-op (we'd rather fail-open than block the modeler).
void rebuildIdentifierCatalog();

// Run validation. Tokenizes the script (skipping comments and string
// literals), finds top-level call shapes, classifies each name as
// known / deprecated / unknown. Empty findings = clean script.
ValidationResult validateIdentifiers(const std::string& script);

} // namespace ModelerAi
