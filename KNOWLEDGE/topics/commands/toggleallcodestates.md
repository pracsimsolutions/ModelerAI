---
id: toggleallcodestates
name: "toggleallcodestates"
kind: command
module: ""
signature: "toggleallcodestates(node startpoint, num ascpp)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "Toggles all code nodes under startpoint as either C++ or flexscript"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# toggleallcodestates

```flexscript
toggleallcodestates(node startpoint, num ascpp)
```

Toggles all code nodes under startpoint as either C++ or flexscript

Traverses the tree starting at start point and toggles all code nodes to either c++ or flexscript. 1 toggles to c++, 0 toggles to flexscript. This will only change nodes that have not been locked to their code state. This is the same as using the menu option Build|Make all code C++ or Build|Make all code flexscript.

## Example

```flexscript
toggleallcodestates(model(), 1);
```

