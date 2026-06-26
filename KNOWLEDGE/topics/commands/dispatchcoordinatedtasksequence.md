---
id: dispatchcoordinatedtasksequence
name: "dispatchcoordinatedtasksequence"
kind: command
module: ""
signature: "dispatchcoordinatedtasksequence(obj tasksequence)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Dispatches the coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dispatchcoordinatedtasksequence

```flexscript
dispatchcoordinatedtasksequence(obj tasksequence)
```

Dispatches the coordinated task sequence

Dispatches the specified coordinated task sequence. Must be called after calling createcoordinatedtasksequence().

## Example

```flexscript
dispatchcoordinatedtasksequence(newts);
```

