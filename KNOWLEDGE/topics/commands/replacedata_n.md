---
id: replacedata_n
name: "replacedata_n"
kind: command
module: ""
signature: "replacedata_n(obj startnode, num value, num replacement) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: replacedata_n."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# replacedata_n

```flexscript
replacedata_n(obj startnode, num value, num replacement) -> num
```

For developer use. Search/Replace number data matched under startnode. Only number data nodes are searched. Returns the total number of matches found.

## Example

```flexscript
replacedata_n(main(),25,42)
```

