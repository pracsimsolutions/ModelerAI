---
id: getquerymatchtablerow
name: "getquerymatchtablerow"
kind: command
module: ""
signature: "getquerymatchtablerow(str/num table, num matchRow) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Table.query"
description: "Deprecated, use Table.query()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getquerymatchtablerow

> ⚠ **DEPRECATED.** Use `Table.query` instead.

```flexscript
getquerymatchtablerow(str/num table, num matchRow) -> num
```

Deprecated, use Table.query()

This command is deprecated. Use Table.query() instead.

Returns the source table's row number associated with a matched row in the last query() result.

table - If a string, it is the name of the table as defined in the FROM
statement. If you used an alias, it should be the table's alias. If a
number, it should be the 1-based index of the table in the FROM
statement. If there is only one table in the query, this should be 1.
For multiple tables, the first table in the FROM is 1, the second is 2,
etc.
matchRow - The 1-based row of the result table of the query.

Note that this command will not work if you use aggregator functions such as MIN(), MAX(), AVG(), etc.
in the SELECT clause. When aggregators are used, this command just returns the number of rows in the
source table.

