---
id: clientclose
name: "clientclose"
kind: command
module: ""
signature: "clientclose(num socket) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Closes the client socket"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clientclose

```flexscript
clientclose(num socket) -> num
```

Closes the client socket

This command closes the client socket specified as socket. It must be called sometime after clientcreate(). Once a socket is closed, nothing can read from it or write to it. The socket number may be reused again by a later call to clientcreate(). This function returns true if the socket closes correctly.

## Example

```flexscript
clientclose(1);
```

