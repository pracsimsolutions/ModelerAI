---
id: socketinit
name: "socketinit"
kind: command
module: ""
signature: "socketinit() -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Initializes background processes that allow Windows Sockets to be used"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# socketinit

```flexscript
socketinit() -> num
```

Initializes background processes that allow Windows Sockets to be used

This command initializes the background processes that allow Windows Sockets to be used. It must be called before any calls to socket commands will work. This function returns true if the initialization is successful. See clientsend() for more information.

## Example

```flexscript
socketinit();
```

