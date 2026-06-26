---
id: item
name: "item"
kind: command
module: ""
signature: "item<no parentheses> -> num/node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "item"
description: "FlexScript command: item."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# item

> ⚠ **DEPRECATED.** Use `item` instead.

```flexscript
item<no parentheses> -> num/node
```

This command is deprecated. This should not be used as a special command with the setitem command anymore. Instead, declare a treenode variable type as item, then use item in your code as any other variable type.

## Example

```flexscript
treenode item = model().subnodes[3]; pt(item.name);
This sets the object that is ranked 3 in the model to item and prints its name to the output console.
```

