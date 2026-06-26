---
id: filebrowse
name: "filebrowse"
kind: command
module: ""
signature: "filebrowse(str extension, str description, str startpath) -> str"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Opens a file selection dialog"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# filebrowse

```flexscript
filebrowse(str extension, str description, str startpath) -> str
```

Opens a file selection dialog

Opens a file selection dialog that allows the user to select the specified file type(s). Different types should be separated by semicolons(;). The dialog's default directory is the path specified. The commands pdir() and cdir() can only be passed as the path parameter in C++.

## Example

```flexscript
string filename = filebrowse("*.fsm; *.fsp; *.fsv; *.fss","FlexSim Files",pdir());
```

