---
id: getpreempt
name: "getpreempt"
kind: command
module: ""
signature: "getpreempt(node tasksequence) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets the preempt value of the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getpreempt

```flexscript
getpreempt(node tasksequence) -> num
```

Gets the preempt value of the tasksequence

Returns the preempt value of the tasksequence. Possible values are: PREEMPT_NOT, PREEMPT_ONLY, PREEMPT_AND_ABORT_ACTIVE, PREEMPT_AND_ABORT_ALL

## Example

```flexscript
if(getpreempt(mytasksequence) == PREEMPT_ONLY) pt("This tasksequence is a preempting task");
```

