---
id: store
name: "store"
kind: command
module: ""
signature: "store()"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "this"
description: "FlexScript command: store."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# store

> ⚠ **DEPRECATED.** Use `this` instead.

```flexscript
store()
```

This command is deprecated. This command is currently deprecated because a faster leaner implementation of the undo functionality has been developed that does not use this command.

It explicitly stores an archived copy of the current model inside the undo folder of the main tree. Refer to undos() command documentation for more information.

## Example

```flexscript
store();
```

