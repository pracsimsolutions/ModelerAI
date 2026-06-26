---
id: executefsnode
name: "executefsnode"
kind: command
module: ""
signature: "executefsnode(node thenode, node c[, node i, node/num/str eventdata, int nobuild])"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "Execute a node as Flexscript"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# executefsnode

```flexscript
executefsnode(node thenode, node c[, node i, node/num/str eventdata, int nobuild])
```

Execute a node as Flexscript

Dynamically execute a node as Flexscript. If the node is not already Flexscript, the nobuild parameter should be 0. Used to parse string data into Flexscript to execute, such as from a global table or label. The c and i parameters can be accessed in the function as c and i. The eventdata parameter can be accessed as eventdata if it is a string or as eventcode (an integer) if it is a number. If eventdata is a node, it can be accessed using tonode(eventdata) in the function.

## Example

```flexscript
executefsnode(current.labels["myFunc"],NULL,NULL,NULL,0)
This executes the script on current's myFunc label, passing NULL into the function for c,i,and eventdata.
```

