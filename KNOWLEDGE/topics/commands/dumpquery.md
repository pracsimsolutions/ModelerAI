---
id: dumpquery
name: "dumpquery"
kind: command
module: ""
signature: "dumpquery(node dumpNode[, num asTable])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Table.query"
description: "Deprecated, use Table.query()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dumpquery

> ⚠ **DEPRECATED.** Use `Table.query` instead.

```flexscript
dumpquery(node dumpNode[, num asTable])
```

Deprecated, use Table.query()

This command is deprecated. Use Table.query() instead.

Dumps the full result table for the last call to query() into a node.
If asTable is 1, it will dump the result as table data. If asTable is
0, it will dump it as bundle data.

