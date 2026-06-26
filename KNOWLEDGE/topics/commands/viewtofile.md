---
id: viewtofile
name: "viewtofile"
kind: command
module: ""
signature: "viewtofile(obj view, str path[, num width, num height])"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Saves the specified view as a bitmap image"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# viewtofile

```flexscript
viewtofile(obj view, str path[, num width, num height])
```

Saves the specified view as a bitmap image

Saves the specified view as a bitmap image to the specified file.

## Example

```flexscript
viewtofile(node("VIEW:/active/ortho"), modeldir() + "screenshot.bmp"));
```

