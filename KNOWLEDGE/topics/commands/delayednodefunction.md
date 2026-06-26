---
id: delayednodefunction
name: "delayednodefunction"
kind: command
module: ""
signature: "delayednodefunction(node thenode, double timeDelay[, par1, par2, ... par10])"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: delayednodefunction."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# delayednodefunction

```flexscript
delayednodefunction(node thenode, double timeDelay[, par1, par2, ... par10])
```

For developer use. This command executes a function that is defined on thenode after a defined delay time. It works the same as the nodefunction() command, except it delays firing the node for the given delay time.

## Example

```flexscript
delayednodefunction(current.myFunc, tonum(item));
```

