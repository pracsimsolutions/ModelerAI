---
id: notifylinecontroller
name: "notifylinecontroller"
kind: command
module: ""
signature: "notifylinecontroller(node object, int controllernum, num output, num sendinglast [0/1])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "For Dynamo use"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# notifylinecontroller

```flexscript
notifylinecontroller(node object, int controllernum, num output, num sendinglast [0/1])
```

For Dynamo use

For Dynamo use. Informs a LineController connected to the object that material has left the line so the Controller can decide if the run is complete. The number of the LineController to be notified is the contollernum parameter. The object should report how much material (of any kind) has left it since the model began running and whether or not the material leaving is the last material in the object.

## Example

```flexscript
notifylinecontroller(current,1,current.stats.output + 1,(current.subnodes.length == 1));
```

