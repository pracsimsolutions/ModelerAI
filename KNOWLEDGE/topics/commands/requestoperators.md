---
id: requestoperators
name: "requestoperators"
kind: command
module: ""
signature: "requestoperators(obj dispatcher, obj station, obj involved, num nrofops, num priority, num preempting)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: requestoperators."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# requestoperators

```flexscript
requestoperators(obj dispatcher, obj station, obj involved, num nrofops, num priority, num preempting)
```

Request a specified number of operators to come to the station object and work on the involved object until released. The involved object will later need to be passed into the freeoperators() command when releasing the operators. The operator(s) being called must either be the object referenced by dispatcher, or be connected to its output ports.
The tasksequence which is sent to the operator(s) will be given a priority and preempting value. The possible values for preempting are as follows:

0 = PREEMPT_NOT
1 = PREEMPT_ONLY
2 = PREEMPT_AND_ABORT_ACTIVE
3 = PREEMPT_AND_ABORT_ALL

For each operater that needs to be called, stopobject() will be called on the station, and a tasksequence that looks like the following will be sent to the dispatcher (or operator) object.

1. put a preempt "bookmark" in the task sequence (TASKTYPE_MILESTONE)
2. travel to the station (TASKTYPE_TRAVEL)
3. resume the station (TASKTYPE_STOPREQUESTFINISH)
4. be utilized at the station (TASKTYPE_UTILIZE)

## Example

```flexscript
requestoperators(current.centerObjects[1], current, item, 2, 0, PREEMPT_NOT);
This calls 2 operators connected to the object that is connected to the first center port of the current object to come to the current object, and work on the item.
```

