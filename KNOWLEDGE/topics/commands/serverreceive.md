---
id: serverreceive
name: "serverreceive"
kind: command
module: ""
signature: "serverreceive(int connection, char *buffer, int bufsize, int noblocking)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Receives a message from the connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# serverreceive

```flexscript
serverreceive(int connection, char *buffer, int bufsize, int noblocking)
```

Receives a message from the connection

Receives a message from the specified connection. This command operates differently in flexscript vs. c++. In c++, buffer must be a valid pointer to a char* destination into which the read bytes will be copied. The return value will be the total number of bytes received. Up to bufsize bytes from the clients message will be read and stored in the preallocated character array called buf. In flexscript, however, buffer should be passed as NULL, and the return value will be the actual string that is received. If noblocking = 1, then this command does not block FlexSim program execution, but if noblocking = 0, FlexSim program execution will block (freeze) until something is received from the client.

## Example

```flexscript
c++: int bytes = serverreceive(1,Buf,15,0);
flexscript: string readstr = serverreceive(1, NULL, 15, 0);
```

