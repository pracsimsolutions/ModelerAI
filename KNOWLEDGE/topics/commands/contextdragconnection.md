---
id: contextdragconnection
name: "contextdragconnection"
kind: command
module: ""
signature: "contextdragconnection(obj fromobject, obj toobject, str/num characterpressed)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Creates an A or S connection between objects"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# contextdragconnection

```flexscript
contextdragconnection(obj fromobject, obj toobject, str/num characterpressed)
```

Creates an A or S connection between objects

This command mimicks holding the specified letter down on the keyboard, clicking on the fromobject and then dragging to the toobject with the mouse. Whatever context sensitive action takes place when using the actual keyboard and mouse combinations, will occur when this command is executed. This command can be used in both Flexscript and C++.

## Example

```flexscript
contextdragconnection(model().subnodes[5], model().subnodes3], "A");
```

