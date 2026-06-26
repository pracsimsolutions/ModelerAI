---
id: getmoverecord
name: "getmoverecord"
kind: command
module: ""
signature: "getmoverecord(num record)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getmoverecord

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
getmoverecord(num record)
```

Deprecated

This command is deprecated. Returns a movement record that has been recorded for the Full History Report. A movement record is created every time a flowitem moves from one object to another if Full History is enabled. The parameter passed to this function is the number of the record (beginning at 1) that the user wishes to retrieve. If this command is called in C++, the return value is a MoveRecord*. If this command is called in FlexScript, the return value can be assigned to an int.

## Example

```flexscript
See the example for getnummoverecords().
```

