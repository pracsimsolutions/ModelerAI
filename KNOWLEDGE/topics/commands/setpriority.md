---
id: setpriority
name: "setpriority"
kind: command
module: ""
signature: "setpriority(obj tasksequence, num newpriority)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Sets the priority of the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setpriority

```flexscript
setpriority(obj tasksequence, num newpriority)
```

Sets the priority of the tasksequence

Sets the priority value of the tasksequence to the value newpriority.

## Example

```flexscript
setpriority(myts, 1);
This example sets the priority value of the task sequence myts to the value of 1.
```

