---
id: addbundleentry
name: "addbundleentry"
kind: command
module: ""
signature: "addbundleentry(node datanode[, field1val, field2val, ...]) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: "setbundlevalue."
description: "Adds a new entry to the bundle (base 0)"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addbundleentry

```flexscript
addbundleentry(node datanode[, field1val, field2val, ...]) -> num
```

Adds a new entry to the bundle (base 0)

Adds a new entry to the bundle (base 0). All fields must be added before adding entries. The optional field values will be added to the fields in the given order. Up to 29 field values can be given. For bundles with more fields, use setbundlevalue.

## Example

```flexscript
int lastentry = addbundleentry(x, time(), current.subnodes.length);
```

