---
id: beginignoreundo
name: "beginignoreundo"
kind: command
module: ""
signature: "beginignoreundo()"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: beginignoreundo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# beginignoreundo

```flexscript
beginignoreundo()
```

Used in conjunction with beginaggregatedundo() and endaggregatedundo(). Any commands called between beginignoreundo() and endignoreundo() will not be added as undoable commands within the aggregated undo record.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
beginignoreundo();
```

