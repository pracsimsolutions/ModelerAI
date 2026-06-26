---
id: continueaggregatedundo
name: "continueaggregatedundo"
kind: command
module: ""
signature: "continueaggregatedundo(obj view) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: continueaggregatedundo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# continueaggregatedundo

```flexscript
continueaggregatedundo(obj view) -> num
```

Continues the previously-finished aggregated undo record for the specified view. This is used if some other logic created and finished an aggregated undo,
but you want it to append any subsequent changes onto the previously finished undo record.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
int id = continueaggregatedundo(c.find("@/TheTable"));
```

