---
id: servercreatemain
name: "servercreatemain"
kind: command
module: ""
signature: "servercreatemain(int port) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Creates a server socket that is listening for connections to the specified port"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# servercreatemain

```flexscript
servercreatemain(int port) -> num
```

Creates a server socket that is listening for connections to the specified port

Creates a server socket that is listening for connections on the specified port. Some ports (called well-known ports) are reserved for special applications. Anything port number over 1024 is usually safe to use.

## Example

```flexscript
servercreatemain(1880);
```

