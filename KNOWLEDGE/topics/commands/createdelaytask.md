---
id: createdelaytask
name: "createdelaytask"
kind: command
module: ""
signature: "createdelaytask(obj dispatcher, num delaytime, num state, num priority, num preempting)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createdelaytask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createdelaytask(obj dispatcher, num delaytime, num state, num priority, num preempting)
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

This command creates a task sequence for the object specified by dispatcher that causes it to pause its current task for delaytime seconds. During the time that it is delayed, it will be in the state specified by state. The task sequence will be given a priority of priority and a preempting value of preempting. The possible values for preempting are:
0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createdelaytask(current.centerObjects[1], 25, SCHEDULED_DOWN, 1, PREEMPT_NOT);
```

