---
id: redirectnetworktraveler
name: "redirectnetworktraveler"
kind: command
module: ""
signature: "redirectnetworktraveler(obj traveler, obj newdest)"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "FlexScript command: redirectnetworktraveler."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# redirectnetworktraveler

```flexscript
redirectnetworktraveler(obj traveler, obj newdest)
```

Attempts to change the end destination of the traveler to that of the newdest object. Only works if the traveler is currently on a TASKTYPE_TRAVEL task and is on a travel network. Returns 1 if successful.

## Example

```flexscript
redirectnetworktraveler(traveler, Model.find("Processor5"));
Redirects the traveler to an object in the model named Processor5 assuming traveler is a pointer to a taskexecuter currently traveling on a network node system and Processor5 is connected to the network.
```

