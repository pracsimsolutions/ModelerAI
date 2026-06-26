---
id: objecttexcode
name: "objecttexcode"
kind: command
module: ""
signature: "objecttexcode(obj object) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "getobjecttextureindex"
description: "Deprecated, use getobjecttextureindex()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# objecttexcode

> ⚠ **DEPRECATED.** Use `getobjecttextureindex` instead.

```flexscript
objecttexcode(obj object) -> num
```

Deprecated, use getobjecttextureindex()

This command is deprecated. Use getobjecttextureindex() instead.

Gets the index of the object's default image. The media list of FlexSim will keep a list of all loaded textures (images). Each texture has a corresponding name and an index. This command will return the index corresponding to the objects default texture (image).

## Example

```flexscript
objecttexcode(current)
```

