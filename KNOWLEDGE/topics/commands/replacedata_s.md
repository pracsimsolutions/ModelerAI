---
id: replacedata_s
name: "replacedata_s"
kind: command
module: ""
signature: "replacedata_s(obj startnode, str string, str replacement[, num replication]) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: replacedata_s."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# replacedata_s

```flexscript
replacedata_s(obj startnode, str string, str replacement[, num replication]) -> num
```

For developer use. Search for a string data match under startnode and replace with the replacement string. Results appear in the output console. This function will recognize and replace substrings. The search is case-sensitive. Replication is currently not used. See searchdata_s(). Returns the total number of matches found.

## Example

```flexscript
replacedata_s(main(),"jack","john")
```

