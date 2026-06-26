---
id: rerankbesttask
name: "rerankbesttask"
kind: command
module: ""
signature: "rerankbesttask(obj object)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: rerankbesttask."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rerankbesttask

```flexscript
rerankbesttask(obj object)
```

This command forces the object specified as dispatcher to search through its task sequence queue and put the task sequence with the highest priority in the front of its queue. This will not reorder the other task sequences in the queue.

## Example

```flexscript
rerankbesttask(current);
```

