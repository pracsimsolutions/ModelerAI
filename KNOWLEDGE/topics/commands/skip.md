---
id: skip
name: "skip"
kind: command
module: ""
signature: "skip()"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "FlexScript command: skip."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# skip

```flexscript
skip()
```

For developer use. Advance the model run by one step (process exactly one event) and then process any and all events at that same exact model time.

## Example

```flexscript
skip()
```

