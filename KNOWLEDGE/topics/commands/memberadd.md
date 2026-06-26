---
id: memberadd
name: "memberadd"
kind: command
module: ""
signature: "memberadd(obj childobject, str parentname, num type)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: memberadd."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# memberadd

```flexscript
memberadd(obj childobject, str parentname, num type)
```

If type = 1, then the childobject is added as a member to a Time Table named "parentname".If type = 2, then the childobject is added as a member to a MTBFMTTR object named "parentname".This is useful for dynamically assigning operators to different shifts for instance.

## Example

```flexscript
memberadd(model().subnodes[2],"TimeTable1",1)
```

