---
id: getpriority
name: "getpriority"
kind: command
module: ""
signature: "getpriority(node tasksequence) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets the priority value of the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getpriority

```flexscript
getpriority(node tasksequence) -> num
```

Gets the priority value of the tasksequence

Returns the priority value of the tasksequence.

## Example

```flexscript
if(getpriority(mytasksequence) > 5) pt("This tasksequence has a priority greater than 5.");
```

