---
id: inputopen
name: "inputopen"
kind: command
module: ""
signature: "inputopen(obj object) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "Returns 1 if the object's input is open, 0 otherwise"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# inputopen

```flexscript
inputopen(obj object) -> num
```

Returns 1 if the object's input is open, 0 otherwise

Returns 1 if the object's input is open, 0 otherwise. The closeinput(), stopinput(), openinput(), and resumeinput() commands affect the return value of this command. The commands like closeip() and closeallip() do NOT affect the return value.

## Example

```flexscript
if(inputopen(current)) current.input.close;
```

