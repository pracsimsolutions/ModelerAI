---
id: pfile
name: "pfile"
kind: command
module: ""
signature: "pfile() -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: pfile."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pfile

```flexscript
pfile() -> str
```

For developer use. Returns the string attached to the end of the command line used to start the FlexSim application.

## Example

```flexscript
If FlexSim was started with this command line:
"C:\Program Files\Flexsim7\program\flexsim.exe" "mymodel.fsm"
then a call to pfile() would return mymodel.fsm as a string.
```

