---
id: memberremoveall
name: "memberremoveall"
kind: command
module: ""
signature: "memberremoveall(str parentname, num type)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: memberremoveall."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# memberremoveall

```flexscript
memberremoveall(str parentname, num type)
```

If type = 1, then all members are removed from a Time Table named "parentname".If type = 2, then all members are removed from a MTBFMTTR object named "parentname".see memberremove()

## Example

```flexscript
memberremoveall("TimeTable1",1)
```

