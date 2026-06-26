---
id: createtraveltolocandwaittask
name: "createtraveltolocandwaittask"
kind: command
module: ""
signature: "createtraveltolocandwaittask(obj taskexecuter, num relativeloc, num x, num y, num z, num endspeed, num state, num priority, num preempting)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createtraveltolocandwaittask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createtraveltolocandwaittask(obj taskexecuter, num relativeloc, num x, num y, num z, num endspeed, num state, num priority, num preempting)
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

This command creates a task sequence for the object specified by dispatcher that causes it to travel to the point specified by xlox, y, and z. Once the object reaches that point, it will wait in the state specified as state until an endwaittask() command is issued to it. For a list of legal states, see setstate(). If relativeloc is 1, the specified point is relative to the origin of dispatcher. If relativeloc is 0, the specified point is the exact point in
the model that dispatcher will try to travel to. It will travel until its front edge reaches the specified point. This task sequence will be given a priority of priority and a preempting value of preempting. The possible values for preempting are:

0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createtraveltolocandwaittask(current.centerObjects[1], 1, 5.15, 2.5, 0, 0, STATE_IDLE, 1, PREEMPT_NOT);
```

