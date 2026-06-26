---
id: getpath
name: "getpath"
kind: command
module: ""
signature: "getpath(str truncatedpath) -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "Returns the full file path of the truncated path"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getpath

```flexscript
getpath(str truncatedpath) -> str
```

Returns the full file path of the truncated path

Takes a truncated, or relative file path, and returns the full file path.

## Example

```flexscript
string fullpath = getpath("fs3d\\texturebox.wrl");
```

