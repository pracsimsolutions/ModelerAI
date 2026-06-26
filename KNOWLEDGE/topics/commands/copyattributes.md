---
id: copyattributes
name: "copyattributes"
kind: command
module: ""
signature: "copyattributes(obj destination, obj source, num mask)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "mask"
description: "FlexScript command: copyattributes."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# copyattributes

```flexscript
copyattributes(obj destination, obj source, num mask)
```

For developer use. Copy the attributes from one object to another. Use mask parameter to enable specific groups (from least significant): ATTCOPY_ALL ATTCOPY_SPATIAL ATTCOPY_OFFSETS ATTCOPY_VISUAL ATTCOPY_MEDIA ATTCOPY_LABELS ATTCOPY_VARIABLES ATTCOPY_DESCRIPTIVE ATTCOPY_TABLES
Use the OR operator (|) to combine these values into a single value for the mask parameter.

## Example

```flexscript
copyattributes(library().subnodes[1],model().subnodes[1],ATTCOPY_SPATIAL | ATTCOPY_VISUAL)
```

