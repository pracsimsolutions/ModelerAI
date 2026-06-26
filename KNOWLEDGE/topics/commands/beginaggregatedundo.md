---
id: beginaggregatedundo
name: "beginaggregatedundo"
kind: command
module: ""
signature: "beginaggregatedundo(obj view, str description) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: beginaggregatedundo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# beginaggregatedundo

```flexscript
beginaggregatedundo(obj view, str description) -> num
```

Begins an aggregated undo record for the specified view. Any undoable commands that are executed between this command and the next endaggregatedundo() command will be encapsulated into a single undoable operation. Returns the id for the created record.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
int id = beginaggregatedundo(c.find("@/TheTable"),"Name Change");
```

