---
id: getutilizedobject
name: "getutilizedobject"
kind: command
module: ""
signature: "getutilizedobject(obj dispatcher, obj involved, obj station, num utilizednr) -> obj"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets a reference to an operator that is currently executing a utilize task"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getutilizedobject

```flexscript
getutilizedobject(obj dispatcher, obj involved, obj station, num utilizednr) -> obj
```

Gets a reference to an operator that is currently executing a utilize task

Finds and returns a reference to an operator that is currently executing a utilize task
with the given involved and station variables. If multiple operators are being used, then utilizednr allows
you to distinguish which one you want to reference.

## Example

```flexscript
treenode op = getutilizedobject(current.centerObjects[1], item, current, 1);
```

