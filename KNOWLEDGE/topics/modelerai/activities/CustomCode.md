---
title: CustomCode activity — variable semantics
status: v1 — hand-curated 2026-06-11 (batch 1 of activity deep-dive)
---

# CustomCode

**What it does:** Executes an arbitrary FlexScript body as the token
passes through. Side-effects only — the return value is ignored. The
token continues to the next connector immediately after the body
finishes.

The "escape hatch" activity — used when no other activity covers what
the modeler needs. Common for ad-hoc debugging, prototyping new
behaviour before a curated activity exists, label-shaping that's too
complex for AssignLabels, or any model mutation that should happen
synchronously with a token's passage.

## Variables

### `codeNode` (dataType 2 → string) — SEMANTIC

The FlexScript body that runs on token arrival. Standard PF codeHeader
is auto-prepended:

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
treenode processFlow = ownerobject(activity);
```

**Body semantics:**
- Return value is **ignored**. Don't `return` anything that matters.
- Side-effects on the model, the token, labels, tables, etc. are the
  whole point.
- **Synchronous** — the token does not delay here. If you need a delay,
  put a Delay activity downstream, or use a coroutine (`co_await
  Delay::realTime(...)`) inside the body. Never use a `while` loop to
  busy-wait — that freezes FlexSim.

**Common patterns** (anything goes; here are typical examples):
- **Label mutation:** `token.labels["status"] = "processed";`
- **Model mutation:** `Object q = Model.find("Queue1"); q.color = Color.red();`
- **Counter increment via a table:** `Table("Counters")[1][1]++;`
- **Diagnostics:** `print("token " + string(token.name) + " arrived at " + string(time()) + "\n");`
- **Staging for downstream Decide:** `token.labels["routeTo"] = (token.labels["priority"].value > 5) ? "fast" : "slow";`

### Inherited / visual — DANGEROUS or USELESS

`container`, `lockedToMe`, `next`, `prev`, etc. — **NEVER set `next`/`prev`**
(use `modelerai_connect_activities`). Visual cosmetics rarely matter.
See the [Delay](Delay.md) topic for the full inherited-vars policy.

## Common pattern

```jsonc
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CustomCode1",
  variable: "codeNode",
  value: "token.labels[\"processed\"] = 1; print(\"processed token \" + string(token.name) + \"\\n\");"
}
```

## Gotchas

- **Return value is ignored** — even if you write `return 42;`, nothing
  reads it. Don't try to "control" flow via the return; use a downstream
  Decide reading a label set here.
- **Synchronous = no delays.** Never put a `while` loop that waits for
  something. Use a downstream Delay or a coroutine if you need to wait.
- **Same codeHeader as Delay / AssignLabels.** Do NOT redeclare
  `current`, `activity`, `token`, `processFlow` in your body — they're
  already in scope.
- **Last resort.** If a curated activity exists for the task (AssignLabels
  for labels, Decide for branching, MoveObject for model-object motion),
  prefer it. CustomCode is the catch-all when nothing else fits.
