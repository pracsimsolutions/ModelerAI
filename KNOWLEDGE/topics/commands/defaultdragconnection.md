---
id: defaultdragconnection
name: "defaultdragconnection"
kind: command
module: ""
signature: "defaultdragconnection(obj fromobject, obj toobject, char characterpressed)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "contextdragconnection"
description: "Deprecated, use contextdragconnection()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# defaultdragconnection

> ⚠ **DEPRECATED.** Use `contextdragconnection` instead.

```flexscript
defaultdragconnection(obj fromobject, obj toobject, char characterpressed)
```

Deprecated, use contextdragconnection()

This command is deprecated. Use contextdragconnection() instead.

Either connects or disconnects ports between fromobject and toobject according to the specified characterpressed. This command mimics the keyboard and mouse combinations used to connect/disconnect object ports. For instance an 'A' would be as if the letter "A" were held down on the keyboard while click-dragging from the fromobject to the toobject. For 'A' and 'Q' it connects and disconnects output ports to input ports. For 'S' and 'W' it connects and disconnects central ports. Can only be used in C++, not flexscript.

## Example

```flexscript
defaultdragconnection(model().subnodes[count], model().subnodes[2], 'A')
```

