---
id: insertsynctask
name: "insertsynctask"
kind: command
module: ""
signature: "insertsynctask(obj tasksequence, num taskkey) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Inserts a sync task into the coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# insertsynctask

```flexscript
insertsynctask(obj tasksequence, num taskkey) -> num
```

Inserts a sync task into the coordinated task sequence

Insert a Sync Task for the specified task in a coordinated task sequence.

## Example

```flexscript
insertsynctask(newts, travelkey);
```

