---
id: switch_variable
name: "switch_variable"
kind: command
module: ""
signature: "switch_variable(obj object, str var[, num off/on]) -> num"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch_variable."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_variable

```flexscript
switch_variable(obj object, str var[, num off/on]) -> num
```

For developer use. Add and set; or remove a dynamic switch variable to/from an object. When set to on, a node called var will be added to the structure of object and have a numerical value of 1 assigned to it. When set to off, a node called var, if present will be removed from the structure of object. Used in model update code.

## Example

```flexscript
switch_variable(so(),"arrivalmode",1);
```

