---
id: openinput
name: "openinput"
kind: command
module: ""
signature: "openinput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.input.open"
description: "Deprecated, use Object.input.open()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# openinput

> ⚠ **DEPRECATED.** Use `Object.input.open` instead.

```flexscript
openinput(obj object)
```

Deprecated, use Object.input.open()

This command is deprecated. Use Object.input.open() instead.

This command unblocks the input of the object after it has been previously blocked with the closeinput() command. It is necessary that both the input ports themselves, and the input in general, are open before entry will be allowed. It is advised that modelers never use this command in any field other than the OnMessage field which has been triggered with the senddelayedmessage() command, because commands which open ports often spawn several other activities that shouldn't be performed during transitional events. Compare this command with resumeinput().

## Example

```flexscript
if (current.subnodes.length < maxcontent) openinput(current);
```

