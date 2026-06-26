---
id: getqueryvalue
name: "getqueryvalue"
kind: command
module: ""
signature: "getqueryvalue(num row, num/str col) -> num/str/node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Table.query"
description: "Deprecated, use Table.query()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getqueryvalue

> ⚠ **DEPRECATED.** Use `Table.query` instead.

```flexscript
getqueryvalue(num row, num/str col) -> num/str/node
```

Deprecated, use Table.query()

This command is deprecated. Use Table.query() instead.

Returns a value in the result table for the last call to query()

row - The 1-based row number of the result table
col - The 1-based column number of the result table, or the name of the
result column. If you have defined aliases in the select statement, the
column name should be the alias.

