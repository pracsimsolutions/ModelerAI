---
id: socketend
name: "socketend"
kind: command
module: ""
signature: "socketend() -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Shuts down the processes started with socketinit()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# socketend

```flexscript
socketend() -> num
```

Shuts down the processes started with socketinit()

This command shuts down the processes that were started with socketinit(). It should be called when the model has completely finished working with the sockets that it had open. This function returns true if the shut down is successful. See clientsend() for more information.

## Example

```flexscript
socketend();
```

