---
id: runprogram
name: "runprogram"
kind: command
module: ""
signature: "runprogram(str commandline) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Run an external program"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# runprogram

```flexscript
runprogram(str commandline) -> num
```

Run an external program

Run an external program. This command will issue a command to the operating system. The commandline may contain full program arguments. A return value > 31 means success.

## Example

```flexscript
runprogram("c:/myprogram.exe")
```

