---
id: getbundlevalue
name: "getbundlevalue"
kind: command
module: ""
signature: "getbundlevalue(node datanode, num entrynr, str/num field) -> num/str/node"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Gets the value of the defined entry (base 0) and field (base 0) of the bundle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getbundlevalue

```flexscript
getbundlevalue(node datanode, num entrynr, str/num field) -> num/str/node
```

Gets the value of the defined entry (base 0) and field (base 0) of the bundle

Returns the value of the defined entry (base 0) and field (base 0) of the bundle.

## Example

```flexscript
string name = getbundlevalue(x, 5, "Object Name");
```

