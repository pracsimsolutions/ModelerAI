---
id: searchdata_n
name: "searchdata_n"
kind: command
module: ""
signature: "searchdata_n(obj startnode, num val) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: searchdata_n."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# searchdata_n

```flexscript
searchdata_n(obj startnode, num val) -> num
```

For developer use. Search for a number data match under startnode. Only number data nodes are searched. Results appear on the output console. Returns the total number of matches found.

## Example

```flexscript
searchdata_n(main(),25)
```

