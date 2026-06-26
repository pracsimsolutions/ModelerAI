---
id: getbundlefieldtype
name: "getbundlefieldtype"
kind: command
module: ""
signature: "getbundlefieldtype(node datanode, num fieldnr) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the data type of the bundle field at rank fieldnr (base 0)"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundlefieldtype

```flexscript
getbundlefieldtype(node datanode, num fieldnr) -> num
```

Gets the data type of the bundle field at rank fieldnr (base 0)

Returns the data type of the bundle field at rank fieldnr (base 0). Available return values are:
BUNDLE_FIELD_TYPE_DOUBLE
BUNDLE_FIELD_TYPE_FLOAT
BUNDLE_FIELD_TYPE_INT
BUNDLE_FIELD_TYPE_STR
BUNDLE_FIELD_TYPE_VARCHAR
BUNDLE_FIELD_TYPE_NODEREF

## Example

```flexscript
int type = getbundlefieldtype(x, 5);
```

