---
id: clearbundle
name: "clearbundle"
kind: command
module: ""
signature: "clearbundle(node datanode[, num resetflags = 0])"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "additional"
description: "Clears all bundle data"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clearbundle

```flexscript
clearbundle(node datanode[, num resetflags = 0])
```

Clears all bundle data

Clears the bundle data associated with datanode. This will clear all entries as well as all fields in the bundle.
resetflags is a bitwise mask for flags to initialize the bundle by. Valid values are:
BUNDLE_FLAG_UNPAGED = 0x1 - The bundle's entries will be stored in an unpaged, contiguous block of memory.
BUNDLE_FLAG_IR_OPT = 0x10 - The bundle will use additional memory (32 bits per row) to improve insert/remove performance for rows in the middle of the bundle.

## Example

```flexscript
clearbundle(x);
```

