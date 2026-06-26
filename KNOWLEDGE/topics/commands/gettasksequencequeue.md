---
id: gettasksequencequeue
name: "gettasksequencequeue"
kind: command
module: ""
signature: "gettasksequencequeue(obj dispatcher) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets a reference to the dispatcher's tasksequence queue"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettasksequencequeue

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettasksequencequeue(obj dispatcher) -> node
```

Gets a reference to the dispatcher's tasksequence queue

This command is deprecated. Use the Dispatcher class properties instead. Returns a reference to the specified dispatcher's task sequence queue.

## Example

```flexscript
treenode tsqueue = gettasksequencequeue(current.centerObjects[1]);
```

