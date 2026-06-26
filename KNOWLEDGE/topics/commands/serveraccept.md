---
id: serveraccept
name: "serveraccept"
kind: command
module: ""
signature: "serveraccept(int noblocking) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Gets the server will attempt to accept a client connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# serveraccept

```flexscript
serveraccept(int noblocking) -> num
```

Gets the server will attempt to accept a client connection

The server will attempt to accept a client connection. If noblocking = 0, the command blocks until a connection is made. It returns an index used to reference the connection made.
The server will attempt to accept a connection from a waiting client. A number will be returned that is the index of the connection established. A return value of 0 indicates that no connection was created. This value will be used in all future server calls. If noblocking = 1, the server will not block FlexSim program execution, but will return a 0 if no client attempts to connect. If noblocking = 0, the server will block (freeze) FlexSim program execution until a client attempts to connect. Connections with clients are only made with this command.

## Example

```flexscript
int client = serveraccept(0);
```

