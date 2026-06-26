---
id: createemptytasksequence
name: "createemptytasksequence"
kind: command
module: ""
signature: "createemptytasksequence(obj dispatcher, num priority, num preempting) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "TaskSequence.create"
description: "Creates a new task sequence in the dispatcher's queue and returns a reference to it"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createemptytasksequence

> ⚠ **DEPRECATED.** Use `TaskSequence.create` instead.

```flexscript
createemptytasksequence(obj dispatcher, num priority, num preempting) -> node
```

Creates a new task sequence in the dispatcher's queue and returns a reference to it

This command is deprecated. Use TaskSequence.create() instead. Creates an empty task sequence in the dispatcher's tasksequence queue. Tasks should be added to this task sequence once it is created, using the inserttask command. The task sequence is dispatched with the dispatchtasksequence command. This command returns a pointer to the newly created tasksequence node. The possible values for preempting are:
0 - PREEMPT_NOT 1 - PREEMPT_ONLY 2 - PREEMPT_AND_ABORT_ACTIVE 3 - PREEMPT_AND_ABORT_ALL

## Example

```flexscript
createemptytasksequence(current.centerObjects[1], 1, PREEMPT_NOT);
```

