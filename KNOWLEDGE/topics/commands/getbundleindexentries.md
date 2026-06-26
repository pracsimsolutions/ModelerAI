---
id: getbundleindexentries
name: "getbundleindexentries"
kind: command
module: ""
signature: "getbundleindexentries(node datanode, str/num field, str/num value)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets all rows with a matching value in the given field"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundleindexentries

```flexscript
getbundleindexentries(node datanode, str/num field, str/num value)
```

Gets all rows with a matching value in the given field

If this command succeeds, it returns an int or an intarray containing
all entries (0-based) that contain the given value for the given field. If it fails,
it returns -1. It will fail if the datanode is not a bundle, or if the given field
is not an indexed field, or if none of values in the field match the given value.
See the addbundlefield command for information on how to make an indexed field.

## Example

```flexscript
int entryNr = getbundleindexentries(data, "SKU", "FS2016-BLU");
```

