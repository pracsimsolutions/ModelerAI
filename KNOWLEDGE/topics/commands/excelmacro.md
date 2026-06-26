---
id: excelmacro
name: "excelmacro"
kind: command
module: ""
signature: "excelmacro(str macro)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelmacro

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
excelmacro(str macro)
```

Deprecated

This command is deprecated. This command allows the user to send an Excel 4.0 macro command to Excel. This language is no longer maintained by Microsoft and may be removed from future versions of Excel.

## Example

```flexscript
excelmacro("select(\"r1c1:r5c5\")");
This selects the section of the workbook beginning at row 1, column 1 to row 5, column 5.
```

