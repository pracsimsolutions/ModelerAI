---
id: pdir
name: "pdir"
kind: command
module: ""
signature: "pdir() -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: pdir."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pdir

```flexscript
pdir() -> str
```

This command returns a string containing the path on the hard drive to the FlexSim's program directory.

## Example

```flexscript
string flexpath = pdir();
This sets flexpath FlexSim's program directory. A typical installation would return C:\Program Files\FlexSim7\program\ as the string.
```

