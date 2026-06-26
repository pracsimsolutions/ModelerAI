---
id: setpreempt
name: "setpreempt"
kind: command
module: ""
signature: "setpreempt(obj tasksequence, num newpreempt)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Sets the preempt value of the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setpreempt

```flexscript
setpreempt(obj tasksequence, num newpreempt)
```

Sets the preempt value of the tasksequence

Sets the preempt value of the tasksequence to the value of newpreempt. Possible values are:
PREEMPT_NOT, PREEMPT_ONLY, PREEMPT_AND_ABORT_ACTIVE, PREEMPT_AND_ABORT_ALL

## Example

```flexscript
setpreempt(tasksequence, PREEMPT_NOT);
This example changes the task sequence's preempt value to not preempt.
```

