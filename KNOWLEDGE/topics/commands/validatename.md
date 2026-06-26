---
id: validatename
name: "validatename"
kind: command
module: ""
signature: "validatename(obj node, num isviewtext [, obj object])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: validatename."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# validatename

```flexscript
validatename(obj node, num isviewtext [, obj object])
```

For developer use. Makes sure the name of node is a valid FlexSim name, replacing invalid characters with underscores and bringing up a message if changes were made. If isviewtext is 1, it assumes the node passed is an edit field, and it changes the viewtext of the field, instead of the name of the node. If the object parameter is specified, it checks that object's name against all the other names of objects in its container. If there are names that are the same, it will pop up an error message. This command is called when you press the Apply or OK buttons in an object's Properties window.

## Example

```flexscript
validatename(current, 0);
```

