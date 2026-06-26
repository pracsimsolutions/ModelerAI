---
id: reftable
name: "reftable"
kind: command
module: ""
signature: "reftable(str/num globaltable)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use Table(\"GlobalTableName\")"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# reftable

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
reftable(str/num globaltable)
```

Deprecated, use Table("GlobalTableName")

This command is deprecated. Use the Table class instead.

Gets a reference to a global table.

## Example

```flexscript
Table table = reftable("GlobalTable1")
```

