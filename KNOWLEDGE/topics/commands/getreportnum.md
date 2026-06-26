---
id: getreportnum
name: "getreportnum"
kind: command
module: ""
signature: "getreportnum(node report, int column, int rowsback) -> num"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "For Dynamo use"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getreportnum

```flexscript
getreportnum(node report, int column, int rowsback) -> num
```

For Dynamo use

For Dynamo use. Returns the value from a Report object that is in the specified column. The row is determined by subtracting the rowsback parameter from the number of rows in the table.

## Example

```flexscript
double myval = getreportnum(current,3,0);
```

