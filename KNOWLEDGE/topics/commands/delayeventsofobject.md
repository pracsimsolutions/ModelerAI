---
id: delayeventsofobject
name: "delayeventsofobject"
kind: command
module: ""
signature: "delayeventsofobject(obj object, num value)"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: delayeventsofobject."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# delayeventsofobject

```flexscript
delayeventsofobject(obj object, num value)
```

For developer use. Delay events of object. The timed events of object are incremented by value, and the eventlist re-sorted. Value may be positive or negative.

## Example

```flexscript
delayeventsofobject(model().subnodes[2],20)
```

