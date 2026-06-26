---
id: getdirbrowse
name: "getdirbrowse"
kind: command
module: ""
signature: "getdirbrowse(str initialdir, str message, node result)"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "Opens a browser window to pick a directory"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getdirbrowse

```flexscript
getdirbrowse(str initialdir, str message, node result)
```

Opens a browser window to pick a directory

Opens a browser window to pick a directory. destnode should have string data, and the directory path will be written
to its text.

## Example

```flexscript
getdirbrowse(documentsdir, "A folder", destnode)
```

