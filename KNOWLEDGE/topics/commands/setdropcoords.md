---
id: setdropcoords
name: "setdropcoords"
kind: command
module: ""
signature: "setdropcoords(obj view[, obj dropnodefrom, obj dropnodeto])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: setdropcoords."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setdropcoords

```flexscript
setdropcoords(obj view[, obj dropnodefrom, obj dropnodeto])
```

For developer use. Sets the dropx() and dropy() coordinate commands for the view based on current mouse location.
You can optionally also set the dropnodefrom and dropnodeto parameters, which will define the nodes returned by dropnodefrom() and dropnodeto().

## Example

```flexscript
setdropcoords()
```

