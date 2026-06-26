---
id: getanimationvar
name: "getanimationvar"
kind: command
module: ""
signature: "getanimationvar(obj object, num/str animation, str varName) -> obj/num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the current value of an animation variable"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getanimationvar

```flexscript
getanimationvar(obj object, num/str animation, str varName) -> obj/num
```

Gets the current value of an animation variable

Gets the current value of an animation variable. See setanimationvar() for more information on animation
variables.

## Example

```flexscript
treenode curItem = getanimationvar(current, "Animation1", "Item1");
```

