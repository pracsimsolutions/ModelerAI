---
id: createsendmessagetask
name: "createsendmessagetask"
kind: command
module: ""
signature: "createsendmessagetask(obj taskexecuter, obj receiver, num priority, num preempting , num param1, num param2, num param3)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createsendmessagetask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createsendmessagetask(obj taskexecuter, obj receiver, num priority, num preempting , num param1, num param2, num param3)
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

This command creates a task sequence on the object specifies as dispatcher that causes it to send a message to the object specified as receiver. The OnMessage trigger of the receiving object will be executed when the message is sent. The values passed as param1, param2, and param3 will be available in the trigger function using the param() command. This task sequence will be given a priority of priority and a preempting value of preempting. The possible values for preempting are:

0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createsendmessagetask(current, current.centerObjects[1],1,PREEMPT_NOT,5, 6, 7);
```

