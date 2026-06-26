---
id: setbundlevalue
name: "setbundlevalue"
kind: command
module: ""
signature: "setbundlevalue(node datanode, num entrynr, str/num field, num/str val)"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Sets the value of the entry (base 0) and field (base 0) of the bundle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setbundlevalue

```flexscript
setbundlevalue(node datanode, num entrynr, str/num field, num/str val)
```

Sets the value of the entry (base 0) and field (base 0) of the bundle

Sets the value of the defined entry (base 0) and field (base 0) of the bundle.

## Example

```flexscript
setbundlevalue(x, 5, "Object Name", current.name);
```

