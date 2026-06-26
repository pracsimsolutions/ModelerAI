---
id: rackgetnroflevels
name: "rackgetnroflevels"
kind: command
module: ""
signature: "rackgetnroflevels( obj rack [,num bay ] ) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the number of levels in the bay of the rack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetnroflevels

```flexscript
rackgetnroflevels( obj rack [,num bay ] ) -> num
```

Gets the number of levels in the bay of the rack

Returns the number of levels in the specified bay of the rack. If no bay parameter is passed into the function, then the nroflevels in the first bay will be returned.

## Example

```flexscript
return duniform(1,rackgetnroflevels(rack, baynum));
This returns a random number between one and the number of levels on the chosen bay of the rack. This is the same as the "Random" option of the place in level pick list.
```

