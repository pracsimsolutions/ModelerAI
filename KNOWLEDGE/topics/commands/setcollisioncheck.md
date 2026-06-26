---
id: setcollisioncheck
name: "setcollisioncheck"
kind: command
module: ""
signature: "setcollisioncheck(obj taskexecuter, num checkstate [, num checkinterval])"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Sets the state and interval for collision checking on the taskexecuter"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setcollisioncheck

```flexscript
setcollisioncheck(obj taskexecuter, num checkstate [, num checkinterval])
```

Sets the state and interval for collision checking on the taskexecuter

Sets the state and interval for collision checking on the taskexecuter. If checkstate is 1, then check for collisions, else don't. If interval is >0, then reset the "Time between Collision Checks" for the taskexecuter specified.

## Example

```flexscript
setcollisioncheck(traveler,1,5);
Starts the traveler checking for collisions every 5 time units.
```

