---
id: destroyevent
name: "destroyevent"
kind: command
module: ""
signature: "destroyevent(node event) -> num"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: destroyevent."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# destroyevent

```flexscript
destroyevent(node event) -> num
```

For developer use. Destroys a particular event from the event list.

## Example

```flexscript
destroyevent(node("MAIN:/1/exec/events").last)
```

