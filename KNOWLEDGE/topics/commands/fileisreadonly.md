---
id: fileisreadonly
name: "fileisreadonly"
kind: command
module: ""
signature: "fileisreadonly(str filename) -> num"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: fileisreadonly."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fileisreadonly

```flexscript
fileisreadonly(str filename) -> num
```

For developer use (Dynamo). Looks for the string "READONLY" in the PROPERTY_COMMENTS of the specified file.

## Example

```flexscript
fileisreadonly(pdir() + "\\flexsim.exe")
```

