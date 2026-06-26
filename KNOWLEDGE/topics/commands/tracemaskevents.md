---
id: tracemaskevents
name: "tracemaskevents"
kind: command
module: ""
signature: "tracemaskevents(num mask)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: tracemaskevents."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# tracemaskevents

```flexscript
tracemaskevents(num mask)
```

For developer use. Set a 32 bit trace mask: Sum of Math.pow(2,N1)+Math.pow(2,N2)... where Nx are the event numbers to be reported in the trace. See View|Trace Debugger...

## Example

```flexscript
tracemaskevents(listsum(Math.pow(2,1),Math.pow(2,2),Math.pow(2,3)))
```

