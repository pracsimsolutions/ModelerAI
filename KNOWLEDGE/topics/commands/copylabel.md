---
id: copylabel
name: "copylabel"
kind: command
module: ""
signature: "copylabel(obj highlightedobject, str/obj labelnamenode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: copylabel."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# copylabel

```flexscript
copylabel(obj highlightedobject, str/obj labelnamenode)
```

For developer use. Copies the label with the name specified by parameter 2 or stored on the string data of the node in parameter 2 from the parameter 1 object to all selected objects in the model.

## Example

```flexscript
copylabel(selectedobject(ownerview(c)), c.find(">labelname"))
```

