---
id: seteventtime
name: "seteventtime"
kind: command
module: ""
signature: "seteventtime(str EventName, num time, num timemode[1/2], num createevent[1/0])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Sets the \"Repeat Event Time\" for the global UserEvent"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# seteventtime

```flexscript
seteventtime(str EventName, num time, num timemode[1/2], num createevent[1/0])
```

Sets the "Repeat Event Time" for the global UserEvent

Sets the "Repeat Event Time" for the specified global UserEvent. If timemode = 1, then timeval will be used directly; otherwise, the timeval - time() will be used. If createevent = 1, then in addition to updating the "Repeat Event Time", a single event will be created at the previous time specified.

## Example

```flexscript
seteventtime("UserEvent1",20,1,0);
The global userevent named "UserEvent1" will have its "Repeat Event Time" to 20.
```

