---
id: autoloadshapes
name: "autoloadshapes"
kind: command
module: ""
signature: "autoloadshapes()"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: autoloadshapes."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# autoloadshapes

```flexscript
autoloadshapes()
```

For developer use. This command loads all 3D shapes required by the current views, library, and model. Shapes are either .3ds or .wrl files. This command may take some time to complete, if there are many shapes that need to be loaded. This command is not available in C++.

## Example

```flexscript
autoloadshapes();
This reloads all of the shapes needed for the current model.
```

