---
id: registereventtype
name: "registereventtype"
kind: command
module: ""
signature: "registereventtype(str name) -> num"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: "with"
description: "FlexScript command: registereventtype."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# registereventtype

```flexscript
registereventtype(str name) -> num
```

Registers a custom event type for use with the Event Log. Returns the index of the eventtype to be used later by other commands such as logevent().

## Example

```flexscript
int index = registereventtype("MyEventType");
```

