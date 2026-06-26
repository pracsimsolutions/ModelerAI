---
id: flyto
name: "flyto"
kind: command
module: ""
signature: "flyto(obj view, num endtime, num x, num y, num z, num rx, num ry, num rz)"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: flyto."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# flyto

```flexscript
flyto(obj view, num endtime, num x, num y, num z, num rx, num ry, num rz)
```

For developer use. Fly to the specified viewpoint over time.

## Example

```flexscript
flyto(sv(),10000,0,0,20,-50,0,-30);
```

