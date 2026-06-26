---
id: userinput
name: "userinput"
kind: command
module: ""
signature: "userinput(obj targetnode, str text [, int stop]) -> var"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Prompts the user for a value in a dialog box"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# userinput

```flexscript
userinput(obj targetnode, str text [, int stop]) -> var
```

Prompts the user for a value in a dialog box

Prompts the user for a value in a dialog box. The value is placed in the specified node. The model does not stop running while the dialog is open unless the stop parameter is specified. If the stop parameter is specified, then the model stops until the dialog is closed and it returns the value.

## Example

```flexscript
userinput(model().subnodes[3].labels["myLabel"],"Please enter a value");
```

