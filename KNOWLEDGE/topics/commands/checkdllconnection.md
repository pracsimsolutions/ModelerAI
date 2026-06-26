---
id: checkdllconnection
name: "checkdllconnection"
kind: command
module: ""
signature: "checkdllconnection(node dllnode, num force) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Returns 1 if the node is properly connected with a dll function"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# checkdllconnection

```flexscript
checkdllconnection(node dllnode, num force) -> num
```

Returns 1 if the node is properly connected with a dll function

Returns 1 if the node is properly connected with a dll function. Otherwise returns 0. If force is 1, then if the node is not connected, it will attempt to bind it to a dll function, and will return 1 if successful, 0 if not. The node must be toggled as DLL and have proper data specifying the path to the dll and the function name.

## Example

```flexscript
if(!checkdllconnection(getvarnode(current, "exittrigger"), 1)) msg("DLL Error","Error connecting to dll.")
This will attempt to connect and object's exit trigger node with a dll function, and will print a message if it is unsuccessful.
```

