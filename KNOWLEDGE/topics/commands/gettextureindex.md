---
id: gettextureindex
name: "gettextureindex"
kind: command
module: ""
signature: "gettextureindex(str name) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Gets the index of a loaded image"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettextureindex

```flexscript
gettextureindex(str name) -> num
```

Gets the index of a loaded image

Get the index of a loaded image by name. The media list of FlexSim will keep a list of all loaded textures (images). Each texture has a corresponding name and an index. This command will return the index corresponding to a texture's name.

## Example

```flexscript
gettextureindex("myimage")
```

