---
id: importtable
name: "importtable"
kind: command
module: ""
signature: "importtable(node importnode, str filename[, num importcolheaders = 0, num importrowheaders = 0, str listseparator = 0, str decimalseparator = 0, str thousandseparator = 0])"
aliases: []
tags: ["command", "group-tables"]
deprecated: false
replacedBy: null
description: "Imports table data into importnode from a csv file. When importing both row and column headers, the first value in the file will be skipped. When importing into a bundle table, each field will use the data type of its first imported cell (row 1)."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# importtable

```flexscript
importtable(node importnode, str filename[, num importcolheaders = 0, num importrowheaders = 0, str listseparator = 0, str decimalseparator = 0, str thousandseparator = 0])
```

Imports table data into importnode from a csv file. When importing both row and column headers, the first value in the file will be skipped. When importing into a bundle table, each field will use the data type of its first imported cell (row 1).

Imports table data into the node from a csv (comma-separated values) file.

## Example

```flexscript
importtable(current.labels["data"], "C:/modeldata.csv", 1, 1)
```

