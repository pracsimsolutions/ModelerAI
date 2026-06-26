---
id: savefpk
name: "savefpk"
kind: command
module: ""
signature: "savefpk(node servicepackhead)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "user"
description: "Deprecated, use user libraries"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# savefpk

> ⚠ **DEPRECATED.** Use `user` instead.

```flexscript
savefpk(node servicepackhead)
```

Deprecated, use user libraries

This command is deprecated. Use user libraries instead.

This saves the node to disk as a service pack file (.fpk). A save file dialog box will appear, allowing you to define the path and name of the file. A service pack is nothing more than a node with subnodes. The first subnode is assumed to be toggled flexscript and contain string data with code. When the service pack is loaded using File | Install Service Pack, the first subnode of you service pack will execute automatically. The code will usually copy the other subnodes into the MAIN and VIEW trees.

## Example

```flexscript
savefpk(so());
```

