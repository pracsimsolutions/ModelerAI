---
id: createwaittask
name: "createwaittask"
kind: command
module: ""
signature: "createwaittask(obj taskexecuter, num state, num priority, num preempting)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createwaittask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createwaittask(obj taskexecuter, num state, num priority, num preempting)
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

This command creates a task sequence on the object specified as dispatcher that causes the object to not perform new task sequences that it is given until it has been released. The object can be released with the endwaittask() command. The object will be in the state specified by state while it is waiting. For a list of possible states, see setstate() command. This task sequence will be given a priority of priority and a preempting value of preempting. The possible values for preempting are:

0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createwaittask(current.centerObjects[1], STATE_IDLE, 1, PREEMPT_NOT);
```

