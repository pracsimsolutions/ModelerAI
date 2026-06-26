---
id: backupfile
name: "backupfile"
kind: command
module: ""
signature: "backupfile(str filepath)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: backupfile."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# backupfile

```flexscript
backupfile(str filepath)
```

For developer use. Backs up the file by appending a ! onto the specified file's name. Note that this doesn't copy the file. You would do this before an overwrite of a file.

## Example

```flexscript
backupfile(modeldir() + "mydocument.htm");
```

