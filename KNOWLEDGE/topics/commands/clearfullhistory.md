---
id: clearfullhistory
name: "clearfullhistory"
kind: command
module: ""
signature: "clearfullhistory(int resetuniqueid)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clearfullhistory

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
clearfullhistory(int resetuniqueid)
```

Deprecated

This command is deprecated. Deletes all of the data that has been collected by the Full History system. If resetuniqueid is 1, then the unique IDs of the flowitems that are created after this command is called will being again at 1. Typically, this parameter should be 0, as that will continue numbering so that old flowitems are not confused with new flowitems.

## Example

```flexscript
clearfullhistory(0);
```

