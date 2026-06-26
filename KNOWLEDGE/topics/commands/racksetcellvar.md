---
id: racksetcellvar
name: "racksetcellvar"
kind: command
module: ""
signature: "racksetcellvar(obj rack,num bay, num level, num varnum, num setvalue, node storagenode)"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: racksetcellvar."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# racksetcellvar

```flexscript
racksetcellvar(obj rack,num bay, num level, num varnum, num setvalue, node storagenode)
```

Sets the value in a three dimensional table with rows representing the rack's bays, columns representing the rack's levels, and sub-nodes of table cells representing different stored values for each cell of the Rack. This allows you to store custom information for each cell of the rack. Bay, level, and varnum reference bay (row), level (column), and variable number (sub-node rank) in the table. Setvalue is the value to set it to.storagenode references the node you have stored this table on. Usually this should be a label on the Rack.

## Example

```flexscript
racksetcellvar(current, 5, 3, 2, 4.6, current.labels["storageData"]);
This sets the 2nd value of bay 5, level 3 to 4.6
```

