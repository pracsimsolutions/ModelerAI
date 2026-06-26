---
id: fileexists
name: "fileexists"
kind: command
module: ""
signature: "fileexists(str filepath) -> num"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: null
description: "Returns 1 if the file exists, 0 otherwise"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fileexists

```flexscript
fileexists(str filepath) -> num
```

Returns 1 if the file exists, 0 otherwise

Returns 1 if the file exists, otherwise 0.

## Example

```flexscript
if(fileexists("C:/myfile.txt")){fileopen("C:/myfile.txt");fpt("Hello World");fileclose();}
```

