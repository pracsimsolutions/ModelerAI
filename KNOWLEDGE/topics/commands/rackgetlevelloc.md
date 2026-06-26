---
id: rackgetlevelloc
name: "rackgetlevelloc"
kind: command
module: ""
signature: "rackgetlevelloc(obj rack, num bay, num level) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the y or z location of the level relative to the rack's origin"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetlevelloc

```flexscript
rackgetlevelloc(obj rack, num bay, num level) -> num
```

Gets the y or z location of the level relative to the rack's origin

Returns the y or z location of the given level of the rack, relative to the rack's origin. If the rack is a floor storage rack, this returns the y rz. If the rack is a normal rack, this returns the z rz.

## Example

```flexscript
rackgetlevelloc(current, 5, 4);
Returns the y or z location of bay 5, level 4.
```

