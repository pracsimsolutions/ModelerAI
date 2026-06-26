---
id: loaduserlibrary
name: "loaduserlibrary"
kind: command
module: ""
signature: "loaduserlibrary(str libfile [, obj libraryview, num reloadmedia, num changeitemcurrent, num autoinstall])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: loaduserlibrary."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loaduserlibrary

```flexscript
loaduserlibrary(str libfile [, obj libraryview, num reloadmedia, num changeitemcurrent, num autoinstall])
```

For developer use. Loads a library from the path specified in libfile. If reloadmedia is true, autoloadallmedia() will be called
after loading the library. If autoinstall is true, the library's load and new model autoinstall objects will be added to the model. The
libraryview and changeitemcurrent parameters are no longer used.

## Example

```flexscript
loaduserlibrary(cdir() + "libraries/mylib.fsl");
```

