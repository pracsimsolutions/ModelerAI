---
id: port
name: "port"
kind: command
module: ""
signature: "port<no parentheses> -> num/node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "port"
description: "FlexScript command: port."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# port

> ⚠ **DEPRECATED.** Use `port` instead.

```flexscript
port<no parentheses> -> num/node
```

This command is deprecated. This should not be used as a special command with the setport command anymore. Instead, declare an int variable type as port, then use port in your code as any other variable type.

## Example

```flexscript
int port = param(3); pd(port);
This sets port to param(3) and then prints the value to the output console.
```

