---
id: openoutput
name: "openoutput"
kind: command
module: ""
signature: "openoutput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.output.open"
description: "Deprecated, use Object.output.open()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# openoutput

> ⚠ **DEPRECATED.** Use `Object.output.open` instead.

```flexscript
openoutput(obj object)
```

Deprecated, use Object.output.open()

This command is deprecated. Use Object.output.open() instead.

This command unblocks the output of the object after it has been previously blocked with the closeoutput() command. It is necessary that both the output ports themselves, and the output in general, are open before exiting is allowed. It is advised that modelers never use this command in any field other than the OnMessage field which has been triggered with the senddelayedmessage() command, because commands which open ports often spawn several other activities that shouldn't be performed during transitional events. Compare this command with resumeoutput().

## Example

```flexscript
if (current.subnodes.length > mincontent) openoutput(current);
```

