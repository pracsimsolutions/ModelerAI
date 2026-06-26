---
id: executefsfile
name: "executefsfile"
kind: command
module: ""
signature: "executefsfile(str filename, num direct)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: executefsfile."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# executefsfile

```flexscript
executefsfile(str filename, num direct)
```

For developer use. Executes Flexscript code in a file. The specified file is loaded and its content is executed. The contents of the file are assumed to be valid Flexscript code. If direct is 0, then FlexSim's program directory will be concatenated onto the front of filename, otherwise filename will remain as it is.

## Example

```flexscript
executefsfile("c:/myscript.fs",1)
```

