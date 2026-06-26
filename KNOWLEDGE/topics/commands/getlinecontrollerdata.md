---
id: getlinecontrollerdata
name: "getlinecontrollerdata"
kind: command
module: ""
signature: "getlinecontrollerdata(obj linecontroller, num datatype, num column) -> num"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "For Dynamo use"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getlinecontrollerdata

```flexscript
getlinecontrollerdata(obj linecontroller, num datatype, num column) -> num
```

For Dynamo use

For Dynamo use. Returns data about the LineController's current job, 1 = ProdID, 2 = LineNum, 3 = Target, 4 = StartTime. 5 = Extra Column

## Example

```flexscript
getlinecontrollerdata(current,1,0);
```

