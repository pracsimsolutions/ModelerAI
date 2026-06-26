---
id: createloadtask
name: "createloadtask"
kind: command
module: ""
signature: "createloadtask(obj taskexecuter, obj destination, obj flowitem, num priority, num preempting)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createloadtask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createloadtask(obj taskexecuter, obj destination, obj flowitem, num priority, num preempting)
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

This command creates a task sequence for the object specified by dispatcher that causes it to load flowitem. The object will not travel to the flowitem before loading it. This task sequence will be given a priority of priority and a preempting value of preempting. The possible values for preempting are:
0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createloadtask(current.centerObjects[1],current.centerObjects[2],item, 1, PREEMPT_NOT);
```

