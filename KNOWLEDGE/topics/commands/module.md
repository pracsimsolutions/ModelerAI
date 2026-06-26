---
id: module
name: "module"
kind: command
module: ""
signature: "module(num type, node libnode)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# module

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
module(num type, node libnode)
```

Deprecated

This command is deprecated. Use the DLL functionality provided with version 4 instead.

Used with dll plugins. The type can be:
MOD_LOAD (1) MOD_UNLOAD (2) MOD_GETFUNCTION (3) MOD_RUNFUNCTION (4)

## Example

```flexscript
module(MOD_UNLOAD,main.find("project/exec/dll/1"))
```

