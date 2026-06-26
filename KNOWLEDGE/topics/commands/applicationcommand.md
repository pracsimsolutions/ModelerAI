---
id: applicationcommand
name: "applicationcommand"
kind: command
module: ""
signature: "applicationcommand(str name[, p1, p2, p3, ...])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: applicationcommand."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# applicationcommand

```flexscript
applicationcommand(str name[, p1, p2, p3, ...])
```

For developer use. Execute a command based on its name. This command will search for the named node in VIEW:/nodefunctions, then in MAIN:/project/exec/globals/nodefunctions, then in VIEW:/commands, and lastly in the FlexSim Engine. If it finds a toggled node, it will call nodefunction on that node passing in up to 10 number parameters. If it finds an untoggled node, it will call executefsnode() on that node passing in n1 as c, n2 as i, and n3 as eventcode.

## Example

```flexscript
applicationcommand("outputconsole");
```

