---
id: getstaterecord
name: "getstaterecord"
kind: command
module: ""
signature: "getstaterecord(num record)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstaterecord

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
getstaterecord(num record)
```

Deprecated

This command is deprecated. Returns a state change record that has been recorded for the Full History Report. A state change record is created every time an object changes state if Full History is enabled. The parameter passed to this function is the number of the record (beginning at 1) that the user wishes to retrieve. If this command is called in C++, the return value is a StateRecord*. If this command is called in FlexScript, the return value can be assigned to an int.

## Example

```flexscript
See the example for getnumstaterecords().
```

