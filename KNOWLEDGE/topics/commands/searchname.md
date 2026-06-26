---
id: searchname
name: "searchname"
kind: command
module: ""
signature: "searchname(obj startnode, str searchname) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: searchname."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# searchname

```flexscript
searchname(obj startnode, str searchname) -> num
```

For developer use. Search for a name match under startnode. Results appear in the output console. The search is case-sensitive. Returns the total number of matches found.

## Example

```flexscript
searchname(main(),"FixedResource")
```

