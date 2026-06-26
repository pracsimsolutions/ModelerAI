---
id: updatestate
name: "updatestate"
kind: command
module: ""
signature: "updatestate(obj object [, num profilenr])"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Updates the state of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# updatestate

```flexscript
updatestate(obj object [, num profilenr])
```

Updates the state of the object

Updates the state of the specified object. Could be used in the OnDraw function of an object to make its states display smoothly while the model is running.

## Example

```flexscript
updatestate(current);
```

