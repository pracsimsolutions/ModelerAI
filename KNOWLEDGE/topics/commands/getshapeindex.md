---
id: getshapeindex
name: "getshapeindex"
kind: command
module: ""
signature: "getshapeindex(str name) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: "double"
description: "Gets the index of a loaded 3D shape"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getshapeindex

```flexscript
getshapeindex(str name) -> num
```

Gets the index of a loaded 3D shape

Get the index of a loaded shape by its file path name. The file path name used in this command must use double backslashes as shown in the example. Use View -> Media Files to see a complete list of currently loaded shapes with their associated file path names and indexes.

## Example

```flexscript
getshapeindex("fs3d\\Operator\\BasicOperator\\Chest.3ds");
```

