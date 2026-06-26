---
id: cellrc
name: "cellrc"
kind: command
module: ""
signature: "cellrc(obj object, num row, num col) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Table.cell"
description: "Deprecated, use Table.cell()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cellrc

> ⚠ **DEPRECATED.** Use `Table.cell` instead.

```flexscript
cellrc(obj object, num row, num col) -> node
```

Deprecated, use Table.cell()

This command is deprecated. Use Table.cell() instead.

Gets a reference to the table's cell.

## Example

```flexscript
double val = getnodenum(cellrc(reftable("MyTable"),2,3));
```

