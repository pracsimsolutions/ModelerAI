---
id: createtickevent
name: "createtickevent"
kind: command
module: ""
signature: "createtickevent(obj fluidobject)"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "Creates a tick event for fluidobject and all objects assigned to that Ticker"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createtickevent

```flexscript
createtickevent(obj fluidobject)
```

Creates a tick event for fluidobject and all objects assigned to that Ticker

Creates a Tick event for the Fluid Object that is passed in and all of the other Fluid Objects that are controlled by the same Ticker. This allows the user to cause fluid material to move at times that are not the Ticker's normal tick times. The user can also pass a specific Ticker as the parameter to this command.

## Example

```flexscript
createtickevent(current);
```

