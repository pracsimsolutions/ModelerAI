---
id: addfullhistorymoverecord
name: "addfullhistorymoverecord"
kind: command
module: ""
signature: "addfullhistorymoverecord(num flowitemid, num time, obj origin, obj destination, num itemtype)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "this"
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addfullhistorymoverecord

> ⚠ **DEPRECATED.** Use `this` instead.

```flexscript
addfullhistorymoverecord(num flowitemid, num time, obj origin, obj destination, num itemtype)
```

Deprecated

This command is deprecated. Adds a new entry to the Movement table that is recorded when Full History is enabled. The new entry records that the flowitem identified by flowitemid moved from the orgin object to the destination object and the specified time. The entry also records that the flowitem's itemtype at the time of the movement was the itemtype passed as the third parameter. Users should only have to use this command in very rare situations.

## Example

```flexscript
addfullhistorymoverecord(getnodenum(uniqueid(item)), time(), current, current.outObjects[1], item.type);
```

