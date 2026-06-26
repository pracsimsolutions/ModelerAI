---
id: rackgetcellvar
name: "rackgetcellvar"
kind: command
module: ""
signature: "rackgetcellvar(obj rack, num bay, num level, num varnum, node storagenode) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: rackgetcellvar."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetcellvar

```flexscript
rackgetcellvar(obj rack, num bay, num level, num varnum, node storagenode) -> num
```

Returns the value stored in a three dimensional table with rows representing the rack's bays, columns representing the rack's levels, and sub-nodes of table cells representing different stored values for each cell of the Rack. This allows you to store custom information for each cell of the rack. Bay, level, and varnum reference bay (row), level (column), and variable number (sub-node rank) in the table. storagenode references the node you have stored this table on. Usually this should be a label on the Rack.

## Example

```flexscript
int typeofcellitems = rackgetcellvar(current, 1,5,3, current.labels["storageData"]);
```

