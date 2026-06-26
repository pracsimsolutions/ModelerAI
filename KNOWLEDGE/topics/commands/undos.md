---
id: undos
name: "undos"
kind: command
module: ""
signature: "undos(num off/on)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "this"
description: "FlexScript command: undos."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# undos

> ⚠ **DEPRECATED.** Use `this` instead.

```flexscript
undos(num off/on)
```

This command is deprecated. This command is currently deprecated because a faster leaner implementation of the undo functionality has been developed that does not use this command.

This command is used to turn on (1) and off (0) the automatic saving of model copies each time there is a mouse click or key press. Use the setundos command, to specify the maximum number of model copies that will be remembered for undoing. To function correctly, this command requires first creating a node in the main tree named undo and giving it number data.

## Example

```flexscript
undos(1);
```

