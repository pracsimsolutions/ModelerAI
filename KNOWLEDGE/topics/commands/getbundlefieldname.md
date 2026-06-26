---
id: getbundlefieldname
name: "getbundlefieldname"
kind: command
module: ""
signature: "getbundlefieldname(node datanode, num fieldnr) -> str"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the name of the nth ranked field (base 0) in the bundle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundlefieldname

```flexscript
getbundlefieldname(node datanode, num fieldnr) -> str
```

Gets the name of the nth ranked field (base 0) in the bundle

Returns the name of the nth ranked field (base 0) in the bundle.

## Example

```flexscript
string fieldname = getbundlefieldname(x, 5);
```

