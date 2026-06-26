---
id: rackgetbayloc
name: "rackgetbayloc"
kind: command
module: ""
signature: "rackgetbayloc(obj rack, num bay) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the x location of bay relative to the rack's origin"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetbayloc

```flexscript
rackgetbayloc(obj rack, num bay) -> num
```

Gets the x location of bay relative to the rack's origin

Returns the x location of the given bay of the rack, relative to the rack's origin.

## Example

```flexscript
rackgetbayloc(current, 5);
Returns the rz of bay 5.
```

