---
id: getnummoverecords
name: "getnummoverecords"
kind: command
module: ""
signature: "getnummoverecords() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnummoverecords

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
getnummoverecords() -> num
```

Deprecated

This command is deprecated. Returns the number of movement records that have been collected for the Full History Report. The records are deleted when a model is reset, so this will return 0 before a model run. If Full History is not enabled, this will return 0.

## Example

```flexscript
// Print the move records that have been recorded
for (int rec = 1; rec <= getnummoverecords(); rec++)
{
int record = getmoverecord(rec);
pd(getmoverecordvalue(record,0)); pt(" ");
pd(getmoverecordvalue(record,1)); pt(" ");
pf(getmoverecordvalue(record,2)); pt(" ");
pt(getmoverecordvalue(record,3).name); pt(" ");
pt(getmoverecordvalue(record,4).name); pt(" ");
pd(getmoverecordvalue(record,5)); pt(" ");
}
```

