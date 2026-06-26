---
id: profiletasksequence
name: "profiletasksequence"
kind: command
module: ""
signature: "profiletasksequence(node tasksequence)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: "menus"
description: "Prints information on a task sequence to the output console"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# profiletasksequence

```flexscript
profiletasksequence(node tasksequence)
```

Prints information on a task sequence to the output console

Prints information on a task sequence to the output console. The output console must be open to write to it. To open the output console, use menus View | Output Console. You can also pass a reference to the TaskExecuter object instead of a specific task sequence. This will profile the object's currently active task sequence.

## Example

```flexscript
profiletasksequence(gettasksequence(so(), 0));
Writes all the tasksequence information to the output console for the active (0) tasksequence of the selected object (so()).
```

