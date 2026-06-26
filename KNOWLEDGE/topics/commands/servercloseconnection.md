---
id: servercloseconnection
name: "servercloseconnection"
kind: command
module: ""
signature: "servercloseconnection(int connection) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Closes the client connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# servercloseconnection

```flexscript
servercloseconnection(int connection) -> num
```

Closes the client connection

The specified connection to a client is closed. Does not close the main server socket. This should be done when the server is completely finished communicating with a client.

## Example

```flexscript
servercloseconnection(1);
```

