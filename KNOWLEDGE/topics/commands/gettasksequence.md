---
id: gettasksequence
name: "gettasksequence"
kind: command
module: ""
signature: "gettasksequence(obj dispatcher, num tasksequencerank) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets a reference to a task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettasksequence

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettasksequence(obj dispatcher, num tasksequencerank) -> node
```

Gets a reference to a task sequence

This command is deprecated. Use the Dispatcher and TaskExecuter class methods and properties instead. Returns a reference to the task sequence that has the specified rank in the specified dispatcher's task sequence queue.If the specified rank is zero, then the command assumes that the dispatcher is actually a TaskExecuter, and it returnsa reference to its currently active task sequence.

## Example

```flexscript
treenode mytasksequence = gettasksequence(current.centerObjects[1], 5);
```

