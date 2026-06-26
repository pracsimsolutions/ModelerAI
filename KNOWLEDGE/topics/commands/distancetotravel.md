---
id: distancetotravel
name: "distancetotravel"
kind: command
module: ""
signature: "distancetotravel(obj traveler, obj destination) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets the distance a traveler would need to travel to reach destination"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# distancetotravel

```flexscript
distancetotravel(obj traveler, obj destination) -> num
```

Gets the distance a traveler would need to travel to reach destination

Returns the distance that a traveler (Operator, Transporter, etc) would need to travel in order to reach the given destination. If the object is connected to a network, then it will return the travel distance along the network.

## Example

```flexscript
distancetotravel(current.centerObjects[1], current);
```

