---
id: endoffile
name: "endoffile"
kind: command
module: ""
signature: "endoffile() -> num"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: null
description: "Returns 1 if the end of the currently open file has been reached"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# endoffile

```flexscript
endoffile() -> num
```

Returns 1 if the end of the currently open file has been reached

Returns 1 if the end of the currently open file has been reached. Used with fileopen()

## Example

```flexscript
while(!endoffile()){string nextline = filereadline();}
```

