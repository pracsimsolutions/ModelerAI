---
id: truncatemediapath
name: "truncatemediapath"
kind: command
module: ""
signature: "truncatemediapath(str path) -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: truncatemediapath."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# truncatemediapath

```flexscript
truncatemediapath(str path) -> str
```

Returns a truncated media path based on the full path of the file passed. If this file exists relative to the model
directory, then the path will be truncated to be relative to the model directory. It will also be truncated if it is relative to
the FlexSim install directory.

## Example

```flexscript
truncatemediapath("C:\\Program Files\\Flexsim4\\fs3d\\texturebox.wrl");
This example will return the string "fs3d\\texturebox.wrl"
```

