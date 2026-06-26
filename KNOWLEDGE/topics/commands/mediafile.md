---
id: mediafile
name: "mediafile"
kind: command
module: ""
signature: "mediafile(str filename) -> str"
aliases: []
tags: ["command"]
deprecated: false
replacedBy: null
description: "FlexScript command: mediafile."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# mediafile

```flexscript
mediafile(str filename) -> str
```

This command will search (by default in fs3d folder) for the filename and returns the full path plus filename if found.

## Example

```flexscript
string fullpath = mediafile("fs3d\\queue_n_color.3ds");
This example will return the string "C:\\Program Files\\Flexsim4\\fs3d\\queue_n_color.3ds"
```

