---
id: pickfile
name: "pickfile"
kind: command
module: ""
signature: "pickfile(str extension, str extensiondescription[, str defaultreturn]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickfile."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickfile

```flexscript
pickfile(str extension, str extensiondescription[, str defaultreturn]) -> num
```

For developer use. Opens a file browser for the user to choose a file. The browser will be looking for files with the specified extension. The extensiondescription is any text which describes the type of files allowed. This command will return the full path to the chosen file. Convert the return value with tostr(). See filebrowse() command.

## Example

```flexscript
pickfile("*.xls","Excel Workbooks")
```

