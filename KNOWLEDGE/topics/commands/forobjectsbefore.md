---
id: forobjectsbefore
name: "forobjectsbefore"
kind: command
module: ""
signature: "forobjectsbefore(node thenode) { }"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "for"
description: "FlexScript command: forobjectsbefore."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# forobjectsbefore

> ⚠ **DEPRECATED.** Use `for` instead.

```flexscript
forobjectsbefore(node thenode) { }
```

This command is deprecated. Use for() or while() commands instead.

Execute a block of code for each node that is in the same layer and has a rank value less than or equal to the rank of the object specified as thenode. To access the node that is currently active in the loop, the "a" command is used. The forobjectsbefore() command can only be used in flexscript, not C++.

## Example

```flexscript
forobjectsbefore(model.subnodes[5])) {pt(a.name); pr(); }
This prints to the outputconsole the name of the first 5 objects in the model.
```

