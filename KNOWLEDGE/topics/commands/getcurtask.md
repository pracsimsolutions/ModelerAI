---
id: getcurtask
name: "getcurtask"
kind: command
module: ""
signature: "getcurtask(node tasksequence)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets the current task number of tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getcurtask

```flexscript
getcurtask(node tasksequence)
```

Gets the current task number of tasksequence

Returns the current task number of the task sequence

## Example

```flexscript
if(getcurtask(ts) > 5) pt("The operator has finished the first five tasks");
```

