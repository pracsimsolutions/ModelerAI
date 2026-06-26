---
id: forobjectsbehind
name: "forobjectsbehind"
kind: command
module: ""
signature: "forobjectsbehind(node thenode) { }"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "for"
description: "FlexScript command: forobjectsbehind."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# forobjectsbehind

> ⚠ **DEPRECATED.** Use `for` instead.

```flexscript
forobjectsbehind(node thenode) { }
```

This command is deprecated. Use for() or while() commands instead.

Execute a block of code for each node that is in the same layer and has a rank value greater than the rank of the object specified as thenode. To access the node that is currently active in the loop, the "a" command is used. The forobjectsbehind() command can only be used in flexscript, not C++.

## Example

```flexscript
forobjectsbehind(model.subnodes[5]) { pt(a.name); pr(); }
This prints to the outputconsole the name of the objects in the model starting with the sixth one.
```

