---
id: addsystemcontrolleritem
name: "addsystemcontrolleritem"
kind: command
module: ""
signature: "addsystemcontrolleritem(obj systemcontroller, num productid, num linenum, num target, num starttime [, num newcolumn1, num newcolumn2, num newcolumn3, num newcolumn4])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "For Dynamo use"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addsystemcontrolleritem

```flexscript
addsystemcontrolleritem(obj systemcontroller, num productid, num linenum, num target, num starttime [, num newcolumn1, num newcolumn2, num newcolumn3, num newcolumn4])
```

For Dynamo use

For Dynamo use. Adds a new line item to a SystemController's schedule during a model run. Does not actually add a new row to the table. The parameters are the same data that the SystemController's schedule requires. The newcolumn parameters are numbers that could have been added to each row of the SystemController's schedule table.

## Example

```flexscript
addsystemcontrolleritem(current.centerObjects[1],0,1,1,0);
```

