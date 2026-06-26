---
id: searchdata_s
name: "searchdata_s"
kind: command
module: ""
signature: "searchdata_s(obj startnode, str string [, num replication]) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: searchdata_s."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# searchdata_s

```flexscript
searchdata_s(obj startnode, str string [, num replication]) -> num
```

For developer use. Search for a string data match under startnode. Results appear in the output console. This function will recognize substrings and report which nodes contain them, and how many. The search is case-sensitive. Replication is currently not used. Returns the total number of matches found.

## Example

```flexscript
searchdata_s(main(),"glVertex()",1)
```

