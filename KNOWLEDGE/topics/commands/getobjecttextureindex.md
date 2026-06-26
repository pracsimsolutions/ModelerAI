---
id: getobjecttextureindex
name: "getobjecttextureindex"
kind: command
module: ""
signature: "getobjecttextureindex(obj object) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the index number of the image assigned as the texture for the object's 3D shape"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getobjecttextureindex

```flexscript
getobjecttextureindex(obj object) -> num
```

Gets the index number of the image assigned as the texture for the object's 3D shape

Returns the index number of the bmp or jpeg image currently assigned as the default texture for the object's 3D shape.

## Example

```flexscript
getobjecttextureindex(item);
```

