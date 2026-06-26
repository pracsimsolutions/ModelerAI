---
id: eventget
name: "eventget"
kind: command
module: ""
signature: "eventget(num index, num thing) -> num/node/str"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "An event attribute for the event at index"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# eventget

```flexscript
eventget(num index, num thing) -> num/node/str
```

An event attribute for the event at index

Returns an event attribute at the specified index in MAIN:/project/exec/events. thing can be: 1 = client 2 = time 3 = code 4 = data 5 = involved 6 = event node

## Example

```flexscript
eventget(2,3)
```

