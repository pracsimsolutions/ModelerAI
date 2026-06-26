---
id: replacename
name: "replacename"
kind: command
module: ""
signature: "replacename(obj startnode, str searchname, str replacement) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: replacename."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# replacename

```flexscript
replacename(obj startnode, str searchname, str replacement) -> num
```

For developer use. Replace all node names found to be a match with searchname that fall under startnode. This function requires an exact match and will not recognize substrings. Returns the total number of matches found.

## Example

```flexscript
replacename(main(),"jack","john")
```

