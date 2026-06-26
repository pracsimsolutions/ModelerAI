---
id: getbundlefieldnr
name: "getbundlefieldnr"
kind: command
module: ""
signature: "getbundlefieldnr(node datanode, str fieldname) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the rank of the bundle field (base 0)"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundlefieldnr

```flexscript
getbundlefieldnr(node datanode, str fieldname) -> num
```

Gets the rank of the bundle field (base 0)

Returns the rank of the bundle field (base 0). If the bundle field doesn't exist, returns -1.

## Example

```flexscript
int index = getbundlefieldnr(x, "Field Name");
```

