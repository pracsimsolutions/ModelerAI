---
id: param
name: "param"
kind: command
module: ""
signature: "param(num index) -> var"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "Gets a parameter passed into a function"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# param

```flexscript
param(num index) -> var
```

Gets a parameter passed into a function

This command is used inside a function that is called by the nodefunction() command. It returns the parameter
passed to nodefunction specified by index as a node (or treenode), string, int, double, stringarray, intarray, doublearray or treenodearray.
The first additional parameter passed to nodefunction() is parameter 1, the second is parameter 2, etc.

## Example

```flexscript
If a trigger/field is called with: nodefunction(thefuncnode, item) then within the trigger/field, getting access to
the item passed as the first additional parameter would be done with:treenode item = param(1);
Other examples include:
double quantity = param(2);
var value = param(1); //In this example value could be used as any type
```

