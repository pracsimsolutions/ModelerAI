---
id: clientconnect
name: "clientconnect"
kind: command
module: ""
signature: "clientconnect(num socket, str hostname, num port) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Attempts to connect the client socket"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clientconnect

```flexscript
clientconnect(num socket, str hostname, num port) -> num
```

Attempts to connect the client socket

This command attempts to connect the socket specified by socket to a server running on the machine called hostname, which is listening on the port specified.
The number passed as socket is the number that was previously returned by clientcreate(). The string passed as hostname can be either the computer's name (if it is on the same network as the client) or its IP address. This command should not be called until after a successful call to clientcreate() is made. If the socket does not connect, then it is closed and clientcreate() will have to be called again to reopen it. This functions returns true if the socket connects successfully.

## Example

```flexscript
clientconnect(1,"localhost",1880);
```

