---
id: closeinput
name: "closeinput"
kind: command
module: ""
signature: "closeinput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.input.close"
description: "Deprecated, use Object.input.close()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeinput

> ⚠ **DEPRECATED.** Use `Object.input.close` instead.

```flexscript
closeinput(obj object)
```

Deprecated, use Object.input.close()

This command is deprecated. Use Object.input.close() instead.

This command blocks the input of the object. Only the input ports themselves, or the input in general needs to be closed to block entry. When port connections are displayed with sufficient size in the view window, you will see a small red bar drawn across all the input ports (squares) for the object, signifying the input ports are blocked. Individual ports may still be open and drawn in green, but entry to the object is still blocked because of the red bar. Compare this command with stopinput().

## Example

```flexscript
if (current.subnodes.length == getvarnum(current,"maxcontent") closeinput(current);
This blocks the input ports of current when it has too many objects inside it.
```

