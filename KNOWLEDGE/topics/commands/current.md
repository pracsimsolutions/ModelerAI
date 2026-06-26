---
id: current
name: "current"
kind: command
module: ""
signature: "current<no parentheses> -> num/node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "current"
description: "FlexScript command: current."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# current

> ⚠ **DEPRECATED.** Use `current` instead.

```flexscript
current<no parentheses> -> num/node
```

This command is deprecated. This should not be used as a special command with the setcurrent command anymore. Instead, declare a treenode variable type as current, then use current in your code as any other variable type.

## Example

```flexscript
treenode current = model().subnodes[3]; pt(current.name);
This sets the object that is ranked 3 in the model to current and prints its name to the output console.
```

