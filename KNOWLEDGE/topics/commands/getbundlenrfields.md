---
id: getbundlenrfields
name: "getbundlenrfields"
kind: command
module: ""
signature: "getbundlenrfields(node datanode) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the total number of fields in the bundle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundlenrfields

```flexscript
getbundlenrfields(node datanode) -> num
```

Gets the total number of fields in the bundle

Returns the total number of fields in the bundle.

## Example

```flexscript
int nrfields = getbundlenrfields(x);
```

