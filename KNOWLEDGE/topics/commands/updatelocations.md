---
id: updatelocations
name: "updatelocations"
kind: command
module: ""
signature: "updatelocations(obj object)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: "this"
description: "Updates the location of the object and the objects inside of it"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# updatelocations

```flexscript
updatelocations(obj object)
```

Updates the location of the object and the objects inside of it

This command causes the object to update the location of itself and possibly the objects inside of it. Use this command to refresh locations before querying or executing functionality that would query the location of an object.

## Example

```flexscript
updatelocations(current);
```

