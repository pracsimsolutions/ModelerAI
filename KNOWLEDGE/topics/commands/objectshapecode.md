---
id: objectshapecode
name: "objectshapecode"
kind: command
module: ""
signature: "objectshapecode(obj object) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "getshapeindex"
description: "Deprecated, use getshapeindex()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# objectshapecode

> ⚠ **DEPRECATED.** Use `getshapeindex` instead.

```flexscript
objectshapecode(obj object) -> num
```

Deprecated, use getshapeindex()

This command is deprecated. Use getshapeindex() instead.

Get the index of the object's default shape. The media list of FlexSim will keep a list of all loaded shapes. Each shape has a corresponding name and an index. This command will return the index corresponding to the objects default shape.

## Example

```flexscript
objectshapecode(so())
```

