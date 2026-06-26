---
id: getdashboardcontrol
name: "getdashboardcontrol"
kind: command
module: ""
signature: "getdashboardcontrol(str id [, node startnode]) -> treenode"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Returns a reference to the dashboard control associated with id"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getdashboardcontrol

```flexscript
getdashboardcontrol(str id [, node startnode]) -> treenode
```

Returns a reference to the dashboard control associated with id

Returns a reference to the dashboard control that has the ID value of id. This command will search through all open dashboards until it finds a control with matching ID. Passing in a value for startnode will cause the command to only search through the dashboard containing startnode. Dashboard controls include edit fields, buttons, comboboxes, etc.

## Example

```flexscript
treenode accelEdit = getdashboardcontrol("accelEdit", c); //Searches through the current dashboard only
```

