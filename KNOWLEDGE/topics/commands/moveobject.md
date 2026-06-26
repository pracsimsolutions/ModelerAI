---
id: moveobject
name: "moveobject"
kind: command
module: ""
signature: "moveobject(obj object, obj location[, num outport])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: "when"
description: "Moves object to the specified location"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# moveobject

```flexscript
moveobject(obj object, obj location[, num outport])
```

Moves object to the specified location

This command moves the object to the specified location through the specified port. The specified port does not have to actually exist, this value is used by the receiving location to be able to reference the port the object came in through. When an object moves into location, the OnReceive event function of location is executed. This includes the OnEntry trigger function. Explicitly calling this on a flowitem with events queued up for it may cause problems. Only use when you know that no events are pending for the object being moved.

## Example

```flexscript
moveobject(current.first,current.centerObjects[1]);
This moves the first object that is inside the object referenced by current to the object connected to current's first center port.
```

