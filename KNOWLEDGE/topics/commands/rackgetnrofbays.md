---
id: rackgetnrofbays
name: "rackgetnrofbays"
kind: command
module: ""
signature: "rackgetnrofbays(obj rack) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the number of bays rack has"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetnrofbays

```flexscript
rackgetnrofbays(obj rack) -> num
```

Gets the number of bays rack has

Returns the number of bays that the rack has.

## Example

```flexscript
return duniform(1,rackgetnrofbays(rack));
This returns a random number between one and the number of bays on the rack. This is the same as the "Random" option of the place in bay pick list.
```

