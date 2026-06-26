---
id: endignoreundo
name: "endignoreundo"
kind: command
module: ""
signature: "endignoreundo()"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: endignoreundo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# endignoreundo

```flexscript
endignoreundo()
```

Used in conjunction with beginaggregatedundo() and endaggregatedundo(). Any commands called between beginignoreundo() and endignoreundo() will not be added as undoable commands within the aggregated undo record.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
endignoreundo();
```

