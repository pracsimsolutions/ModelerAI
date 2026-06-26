---
id: outputopen
name: "outputopen"
kind: command
module: ""
signature: "outputopen(obj object) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "Returns 1 if the object's output is open, 0 otherwise"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# outputopen

```flexscript
outputopen(obj object) -> num
```

Returns 1 if the object's output is open, 0 otherwise

Returns 1 if the object's output is open, 0 otherwise. The closeoutput(), stopoutput(), openoutput(), and resumeoutput() commands affect the return value of this command. The commands like closeop() and closeallop() do NOT affect the return value.

## Example

```flexscript
if(outputopen(current)) current.output.close;
If the output is open for the current object, then close the output.
```

