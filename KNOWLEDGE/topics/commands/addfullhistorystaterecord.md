---
id: addfullhistorystaterecord
name: "addfullhistorystaterecord"
kind: command
module: ""
signature: "addfullhistorystaterecord(num time, num state, obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addfullhistorystaterecord

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
addfullhistorystaterecord(num time, num state, obj object)
```

Deprecated

This command is deprecated. Adds a new entry to the State table that is recorded when Full History is enabled. The new entry records that the specified object passed in as the third parameter changed to the specified state at the time that is entered as the first parameter. This should only need to be called if the object is having its state changed by soem command other than setstate().

## Example

```flexscript
addfullhistorystaterecord(time(), STATE_IDLE, current);
```

