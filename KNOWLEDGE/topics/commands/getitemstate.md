---
id: getitemstate
name: "getitemstate"
kind: command
module: ""
signature: "getitemstate(obj item) -> num"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Gets the FixedResource state of the flowitem"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getitemstate

```flexscript
getitemstate(obj item) -> num
```

Gets the FixedResource state of the flowitem

Returns the FixedResource state of the flowitem. This will be one of 3 values: FRSTATE_INQUEUE: The flowitem is in-process; it has not been released yet.FRSTATE_READY: The flowitem has been release, but has not found a downstream object that is ready to receive it.FRSTATE_MOVEREQUESTED: The flowitem has been released, and a downstream object is ready to receive it, but the item is waiting to get picked up by a transport. For more information, refer to the FixedResource documentation.

## Example

```flexscript
getitemstate(so())
```

