---
id: clientreceive
name: "clientreceive"
kind: command
module: ""
signature: "clientreceive(num socket, str buffer, num buffersize, num noblock[1/0])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Receives data sent to the socket"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clientreceive

```flexscript
clientreceive(num socket, str buffer, num buffersize, num noblock[1/0])
```

Receives data sent to the socket

This command receives data that has been sent to the socket specified as socket. The value of socket is the number that is returned by clientcreate(). This command operates differently in flexscript vs. c++. In c++, buffer must be a valid pointer to a char* destination into which the read bytes will be copied. The return value will be the total number of bytes received. Up to bufsize bytes from the clients message will be read and stored in the preallocated character array called buf. In flexscript, however, buffer should be passed as NULL, and the return value will be the actual string that is received. If noblocking = 1, then this command does not block FlexSim program execution, but if noblocking = 0, FlexSim program execution will block (freeze) until something is received from the client.

## Example

```flexscript
c++: int bytes = clientreceive(1,Buf,15,0);
flexscript: string readstr = clientreceive(1, NULL, 15, 0);
```

