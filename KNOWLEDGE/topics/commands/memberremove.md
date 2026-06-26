---
id: memberremove
name: "memberremove"
kind: command
module: ""
signature: "memberremove(obj childobject, str parentname, num type)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: memberremove."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# memberremove

```flexscript
memberremove(obj childobject, str parentname, num type)
```

If type = 1, then the childobject is removed as a member from a Time Table named "parentname".If type = 2, then the childobject is removed as a member from a MTBFMTTR object named "parentname".see memberadd()

## Example

```flexscript
memberremove(model().subnodes[2],"TimeTable1",1)
```

