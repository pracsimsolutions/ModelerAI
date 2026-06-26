---
id: userdatadir
name: "userdatadir"
kind: command
module: ""
signature: "userdatadir() -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "Gets the full path to the directory where FlexSim stores user data"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# userdatadir

```flexscript
userdatadir() -> str
```

Gets the full path to the directory where FlexSim stores user data

Returns a full path to the directory where FlexSim stores user data, such as global preferences and compiler settings.

## Example

```flexscript
userdatadir() returns a string like "C:\Users\USERNAME\AppData\Roaming\Flexsim\Flexsim7\"
```

