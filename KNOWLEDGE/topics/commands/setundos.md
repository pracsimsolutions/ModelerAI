---
id: setundos
name: "setundos"
kind: command
module: ""
signature: "setundos(num maxundos)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "this"
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setundos

> ⚠ **DEPRECATED.** Use `this` instead.

```flexscript
setundos(num maxundos)
```

Deprecated

This command is deprecated. This command is currently deprecated because a faster leaner implementation of the undo functionality has been developed that does not use this command.

Sets the maximum number of allowable undos. A copy of the entire model will be saved each time a mouse is clicked or a key is pressed. Using the setundos command, you specify the maximum number of model copies that will be remembered for undoing. This command first requires creating a node in the main tree named undo and giving it number data.

## Example

```flexscript
setundos(2);
```

