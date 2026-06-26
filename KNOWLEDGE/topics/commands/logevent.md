---
id: logevent
name: "logevent"
kind: command
module: ""
signature: "logevent(num/str index/name[, obj object, obj involved, num p1, num p2, num p3, num p4])"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: "the"
description: "FlexScript command: logevent."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# logevent

```flexscript
logevent(num/str index/name[, obj object, obj involved, num p1, num p2, num p3, num p4])
```

Logs an event as an entry in the Event Log. Use the name passed into registereventtype() or the index returned by registereventtype().

## Example

```flexscript
logevent("MyEventType",current,item,port);
```

