---
id: stringreturn
name: "stringreturn"
kind: command
module: ""
signature: "stringreturn(str returnstring, ptr callpoint)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: stringreturn."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stringreturn

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
stringreturn(str returnstring, ptr callpoint)
```

This command is deprecated. You can simply return strings from C++ user commands with the return statement.
Prior to FlexSim 7.7, this command was used to return a string from a C++ implementation of a user command. This command was only needed in C++. In FlexScript user commands, you can simply return the string with the return statement.

## Example

```flexscript
C++: stringreturn(result, callpoint); return 0;
```

