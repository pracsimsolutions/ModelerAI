---
id: setobjectimageindex
name: "setobjectimageindex"
kind: command
module: ""
signature: "setobjectimageindex(obj object, num index)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, do not use."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setobjectimageindex

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
setobjectimageindex(obj object, num index)
```

Deprecated, do not use.

This command is deprecated. Do not use.

Sets the index number of the bmp or jpeg image to be assigned to the base of the object (2D shape and base image). For the new index to take effect, the imagebase node of the object must be removed or renamed, so this command renames the imagebase node to "_imagebase" for you. The index number must be a valid index number of a currently imported bmp or jpeg image. To view currently imported images, or to import another bmp or jpeg image, use the Import Media option in the ToolBox.

## Example

```flexscript
setobjectimageindex(item,gettextureindex("fs3d\\BBsphere.bmp"));
```

