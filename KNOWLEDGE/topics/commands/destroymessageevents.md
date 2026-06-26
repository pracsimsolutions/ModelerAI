---
id: destroymessageevents
name: "destroymessageevents"
kind: command
module: ""
signature: "destroymessageevents(obj object, num time, obj fromobj[, num param1, num param2, num param3])"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "Destroys message events associated with object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# destroymessageevents

```flexscript
destroymessageevents(obj object, num time, obj fromobj[, num param1, num param2, num param3])
```

Destroys message events associated with object

Destroys message events associated with the given object.

If no message parameters are included, all message events will be destroyed.

## Example

```flexscript
destroymessageevents(current, time(), NULL, 8)
```

