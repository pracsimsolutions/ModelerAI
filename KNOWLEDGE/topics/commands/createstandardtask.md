---
id: createstandardtask
name: "createstandardtask"
kind: command
module: ""
signature: "createstandardtask(obj taskexecuter, obj origin, obj destination, obj flowitem, num priority, num preempting)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createstandardtask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createstandardtask(obj taskexecuter, obj origin, obj destination, obj flowitem, num priority, num preempting)
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

This command creates a task sequence for the object specified by dispatcher that causes it to travel to origin, load flowitem, travel to destination and unload flowitem. This task sequence will be given a priority of priority and a preempting value of preempting. The possible values for preempting are:

0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createstandardtaskcurrent.centerObjects[1],current.centerObjects[2],current.centerObjects[3],item, 1, PREEMPT_NOT);
```

