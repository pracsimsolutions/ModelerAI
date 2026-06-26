---
id: serversend
name: "serversend"
kind: command
module: ""
signature: "serversend(int connection, char* message [, num charlength] ) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Sends a message to the client connect to connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# serversend

```flexscript
serversend(int connection, char* message [, num charlength] ) -> num
```

Sends a message to the client connect to connection

Sends the specified text message to the client that is connected to the specified connection. Returns the number of bytes (characters) sent correctly. The text message to be sent must be converted into a char *. The charlength is the length of the string being passed and is usually optional. Needed only if the string being sent has NULL characters in it.

## Example

```flexscript
serversend(1,"MadeConnection");
```

