---
id: filereadline
name: "filereadline"
kind: command
module: ""
signature: "filereadline() -> str"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: null
description: "Reads a line from the currently open file"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# filereadline

```flexscript
filereadline() -> str
```

Reads a line from the currently open file

Reads a line from the currently open file. Used with fileopen()

## Example

```flexscript
string nextline = filereadline();
```

