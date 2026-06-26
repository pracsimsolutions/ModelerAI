---
id: clientsend
name: "clientsend"
kind: command
module: ""
signature: "clientsend(num socket, str message[, num charlength]) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Sends a message across the socket"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clientsend

```flexscript
clientsend(num socket, str message[, num charlength]) -> num
```

Sends a message across the socket

This command sends a message across the socket specified as socket. The value passed as socket is the number returned by clientcreate(). The charlength is the length of the string being passed and is usually optional. Needed only if the string being sent has NULL characters in it.

## Example

```flexscript
clientsend(1,"REQ:service");
```

