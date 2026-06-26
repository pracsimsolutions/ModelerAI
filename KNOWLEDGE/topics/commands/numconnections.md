---
id: numconnections
name: "numconnections"
kind: command
module: ""
signature: "numconnections() -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "Gets the number of server connections that are currently active"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# numconnections

```flexscript
numconnections() -> num
```

Gets the number of server connections that are currently active

Returns the number of server connections that are currently active.

## Example

```flexscript
int numconns = numconnections();
Sets the numconns variable to the number of server connections that are currently active.
```

