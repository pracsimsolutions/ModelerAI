---
id: geteventtime
name: "geteventtime"
kind: command
module: ""
signature: "geteventtime(str EventName, num mode[1/2/3]) -> num"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Gets a UserEvent's event time"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# geteventtime

```flexscript
geteventtime(str EventName, num mode[1/2/3]) -> num
```

Gets a UserEvent's event time

Returns a UserEvent's event time. Mode: 1 - Repeat time intervals, 2 - Exact event time, 3 - Time until event fires

## Example

```flexscript
double remainingtime = geteventtime("myevent",3);
```

