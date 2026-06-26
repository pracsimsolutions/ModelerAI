---
id: freeoperators
name: "freeoperators"
kind: command
module: ""
signature: "freeoperators(obj taskexecuter, obj involved)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Release all operators connected to taskexecuter that are working on involved"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# freeoperators

```flexscript
freeoperators(obj taskexecuter, obj involved)
```

Release all operators connected to taskexecuter that are working on involved

Release all operators connected to the specified dispatcher that are currently working on the involved object. This command should be executed from a trigger when you want to release operator(s) that you have called to a process using the requestoperators command or using a utilize task. The first parameter can be either a dispatcher connected to operator(s) that you want to free, or it can be the operator itself. The second parameter is a key for releasing the appropriate operator(s). If you have called the operator(s) using the requestoperators command, then the second parameter of the freeoperators command should match the third parameter of the requestoperators command. If you have given an operator a utilize task, then the second parameter of the freeoperators command should match the involved1 parameter of the utilize task. You would probably execute this command from a trigger like OnProcessFinish or OnSetupFinish. For more information, refer to the task sequence section of the user manual.

## Example

```flexscript
freeoperators(current.centerObjects[1], item);
```

