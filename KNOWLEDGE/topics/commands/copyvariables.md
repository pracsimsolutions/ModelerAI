---
id: copyvariables
name: "copyvariables"
kind: command
module: ""
signature: "copyvariables(obj fromobject)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: copyvariables."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# copyvariables

```flexscript
copyvariables(obj fromobject)
```

For developer use. Copies all variables from the parameter 1 object to all selected objects in the model.

## Example

```flexscript
copyvariables(selectedobject(ownerview(c)))
```

