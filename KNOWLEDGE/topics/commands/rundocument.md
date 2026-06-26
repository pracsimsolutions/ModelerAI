---
id: rundocument
name: "rundocument"
kind: command
module: ""
signature: "rundocument(str command, str path+filename) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Open a file using the default application for the file type"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rundocument

```flexscript
rundocument(str command, str path+filename) -> num
```

Open a file using the default application for the file type

Opens a file using the default application used for opening files with the specified extension on your computer. The available commands are: edit, explore, find, open, and print. A return value > 32 means success.

## Example

```flexscript
rundocument("open","C:/myfile.doc");
Opens myfile.doc with whatever application is default for opening *.doc files on your computer.
```

