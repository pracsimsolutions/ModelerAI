---
id: loadimage
name: "loadimage"
kind: command
module: ""
signature: "loadimage(str file, str name) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: loadimage."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loadimage

```flexscript
loadimage(str file, str name) -> num
```

For developer use. Load an image directly. An image file on disk will be loaded into FlexSim and recorded in the Media List. The media list of FlexSim will keep a list of all loaded images. Each image has a corresponding name and an index. This command will return the index corresponding to the loaded image.

## Example

```flexscript
loadimage("c:/myimage.bmp","myimage")
```

