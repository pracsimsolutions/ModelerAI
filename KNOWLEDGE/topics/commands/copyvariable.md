---
id: copyvariable
name: "copyvariable"
kind: command
module: ""
signature: "copyvariable(obj fromobject, obj variablenamenode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: copyvariable."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# copyvariable

```flexscript
copyvariable(obj fromobject, obj variablenamenode)
```

For developer use. Copies the variable with the name held by the node in parameter 2 from the parameter 1 object to all selected objects in the model.

## Example

```flexscript
copyvariable(selectedobject(ownerview(c)), c.find(">variablename"))
```

