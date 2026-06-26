---
id: count
name: "count"
kind: command
module: ""
signature: "count()"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: count."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# count

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
count()
```

The value of the iterator used with several commands such as repeat(), findmatch(), findmin(), and findmax(). The value of count will depend on which iteration is currently being executed.

## Example

```flexscript
findmax(Table("MyTable").numRows, Table("MyTable")[count][5])
This example returns the maximum value found in column 5, among all the rows of the table MyTable.
```

