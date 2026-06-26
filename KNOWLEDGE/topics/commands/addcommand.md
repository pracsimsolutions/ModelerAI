---
id: addcommand
name: "addcommand"
kind: command
module: ""
signature: "addcommand(obj codenode, str commandname, str description, str parameters, str example [, str cppprefix, str shortdescription])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: addcommand."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addcommand

```flexscript
addcommand(obj codenode, str commandname, str description, str parameters, str example [, str cppprefix, str shortdescription])
```

For developer use. Adds a Flexscript command to the current list of available commands. The new command is available until the next compile. The Tools | User Commands menu is for end-users to create their own commands.

## Example

```flexscript
addcommand(...)
```

