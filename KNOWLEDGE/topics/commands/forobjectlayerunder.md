---
id: forobjectlayerunder
name: "forobjectlayerunder"
kind: command
module: ""
signature: "forobjectlayerunder(node container) { }"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "for"
description: "FlexScript command: forobjectlayerunder."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# forobjectlayerunder

> ⚠ **DEPRECATED.** Use `for` instead.

```flexscript
forobjectlayerunder(node container) { }
```

This command is deprecated. Use for() or while() commands instead.

Execute a block of code for each node that is in the tree layer directly under the node specified as container. To access the node that is currently active in the loop, the "a" command is used. The forobjectlayerunder() command can only be used in flexscript, not C++.

## Example

```flexscript
forobjectlayerunder(model()) { pt(a.name); pr(); }

This prints to the outputconsole the name of every object in the first level of the model.
```

