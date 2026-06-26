---
id: autoloadimages
name: "autoloadimages"
kind: command
module: ""
signature: "autoloadimages()"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: autoloadimages."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# autoloadimages

```flexscript
autoloadimages()
```

For developer use. This command loads all of the images required by the current views, library, and model. Images are either .bmp or .jpg graphics files. This command may take some time to complete, if there are many images that need to be loaded. This command is not available in C++.

## Example

```flexscript
autoloadimages();
This reloads all of the images needed for the current model.
```

