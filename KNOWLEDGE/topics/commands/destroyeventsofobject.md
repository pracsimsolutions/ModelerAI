---
id: destroyeventsofobject
name: "destroyeventsofobject"
kind: command
module: ""
signature: "destroyeventsofobject(obj object [, num simtime, num code, str data, node involved])"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "Destroys events associated with object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# destroyeventsofobject

```flexscript
destroyeventsofobject(obj object [, num simtime, num code, str data, node involved])
```

Destroys events associated with object

Destroys events associated with the given object. The first parameter, object, is the only required parameter. Other parameters are optional and provide a filter for choosing which events to destroy, as follows:

time: If greater than or equal to the time that that the command is called, this parameter will filter events by their time. If the event's time is not equal to the time passed in, then the event will not be destroyed. Pass -1 if you don't want this as a filter.

code: If greater than 0, this parameter filters events by their event code. If the event's code is not equal to the code passed in, the event will not be destroyed.

data: If not NULL, this parameter filters events by the data string associated with the event. If the event's data string is not equal to the string passed in, the event will not be destroyed.

involved: If not NULL, this parameter filters events by their involved node or object. If the event's involved node is not equal to the node passed in, the event will not be destroyed.

## Example

```flexscript
destroyeventsofobject(model().subnodes[2])
```

