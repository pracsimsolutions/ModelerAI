---
id: function_s
name: "function_s"
kind: command
module: ""
signature: "function_s(obj object, str name [, num param_a,...])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: function_s."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# function_s

```flexscript
function_s(obj object, str name [, num param_a,...])
```

For developer use. Executes code associated with the node found in the object's eventfunctions group having the specified name. For a list of the pre-defined system level events in FlexSim refer to function_n() command documentation.

## Example

```flexscript
function_s(current,"OnReset",1,2,3)

Executes the OnReset event function defined on the current object.
```

