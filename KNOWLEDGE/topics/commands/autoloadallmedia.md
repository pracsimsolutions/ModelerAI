---
id: autoloadallmedia
name: "autoloadallmedia"
kind: command
module: ""
signature: "autoloadallmedia([node thenode])"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: autoloadallmedia."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# autoloadallmedia

```flexscript
autoloadallmedia([node thenode])
```

For developer use. This command loads all of the media (images and shapes) required by the current views, library, and model. Images are either .bmp or .jpg graphics files. Shapes are either .3ds or .wrl files. This command may take some time to complete, if there are many images or shapes that need to be loaded. This command is not available in C++.

## Example

```flexscript
autoloadallmedia();
This reloads all of the media needed for the current model.
```

