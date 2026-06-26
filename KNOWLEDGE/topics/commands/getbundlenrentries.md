---
id: getbundlenrentries
name: "getbundlenrentries"
kind: command
module: ""
signature: "getbundlenrentries(node datanode) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the total number of entries in the bundle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundlenrentries

```flexscript
getbundlenrentries(node datanode) -> num
```

Gets the total number of entries in the bundle

Returns the total number of entries in the bundle.

## Example

```flexscript
int nrentries = getbundlenrentries(x);
```

