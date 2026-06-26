---
id: serverclosemain
name: "serverclosemain"
kind: command
module: ""
signature: "serverclosemain() -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Closes the main server socket and all open connections"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# serverclosemain

```flexscript
serverclosemain() -> num
```

Closes the main server socket and all open connections

Closes the main server socket and all of the connections that are still open. No more communication can be done until it is created again. All connections should be closed before this is called.

## Example

```flexscript
serverclosemain()
```

