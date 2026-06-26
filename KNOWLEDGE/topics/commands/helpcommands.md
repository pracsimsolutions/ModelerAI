---
id: helpcommands
name: "helpcommands"
kind: command
module: ""
signature: "helpcommands(node destnode, num what, str command)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: helpcommands."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# helpcommands

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
helpcommands(node destnode, num what, str command)
```

For developer use. Used to get information about a command. Enter the command name in parameter 3.
The return value will be copied into the string data of destnode.
"what" defines what information is needed, as follows:
1 - returns the xml fragment string that contains the documentation for that command

## Example

```flexscript
helpcommands(
```

