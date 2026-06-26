---
id: clientcreate
name: "clientcreate"
kind: command
module: ""
signature: "clientcreate() -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Creates a new client socket with TCP/IP protocol"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clientcreate

```flexscript
clientcreate() -> num
```

Creates a new client socket with TCP/IP protocol

This command creates a new client socket that is set to communicate using the TCP/IP protocol. FlexSim can create up to fifty client sockets at one time.
Each client can be connected to a different server. This function returns a number that must be stored by the model to reference the socket that was created.
If an error occurs, this function returns 0.

## Example

```flexscript
int socknum = clientcreate();
```

