---
id: getnumstaterecords
name: "getnumstaterecords"
kind: command
module: ""
signature: "getnumstaterecords() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnumstaterecords

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
getnumstaterecords() -> num
```

Deprecated

This command is deprecated. Returns the number of state change records that have been collected for the Full History Report. The records are deleted when a model is reset, so this will return 0 before a model run. If Full History is not enabled, this will return 0.

## Example

```flexscript
// Print the state change records that have been recorded
for (int rec = 1; rec <= getnumstaterecords(); rec++)
{
int record = getstaterecord(rec);
pd(getstaterecordvalue(record,0)); pt(" ");
pf(getstaterecordvalue(record,1)); pt(" ");
pd(getstaterecordvalue(record,2)); pt(" ");
pt(getstaterecordvalue(record,3).name); pr();
}
```

