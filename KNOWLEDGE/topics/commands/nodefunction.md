---
id: nodefunction
name: "nodefunction"
kind: command
module: ""
signature: "nodefunction(node thenode [, par1, par2, ... par10])"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: nodefunction."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# nodefunction

```flexscript
nodefunction(node thenode [, par1, par2, ... par10])
```

For developer use. This command executes a function that is defined on thenode. Thenode must be toggled as either a C++ node or a Flexscript node. If the node is C++, the model must be compiled before calling this command. Any parameters that need to be passed to the function are passed as par1, par2, etc. Up to 10 parameters may be passed. The values that are passed using this command must all be numbers, but can then be accessed and recast from within the function called using the command param(x) where x is the number of the parameter.

## Example

```flexscript
nodefunction(node_v_entrytrigger,tonum(item),portnum);
```

