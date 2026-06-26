---
id: endaggregatedundo
name: "endaggregatedundo"
kind: command
module: ""
signature: "endaggregatedundo(obj view, num id)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: endaggregatedundo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# endaggregatedundo

```flexscript
endaggregatedundo(obj view, num id)
```

Ends an aggregated undo record for the specified view. Any undoable commands that are executed between this command and the previous beginaggregatedundo() command will be encapsulated into a single undoable operation. Pass the id returned by the beginaggregatedundo() function.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
endaggregatedundo(c.find("@/TheTable"),id);
```

