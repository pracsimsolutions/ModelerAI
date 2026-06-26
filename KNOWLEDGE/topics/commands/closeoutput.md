---
id: closeoutput
name: "closeoutput"
kind: command
module: ""
signature: "closeoutput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.output.close"
description: "Deprecated, use Object.output.close()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeoutput

> ⚠ **DEPRECATED.** Use `Object.output.close` instead.

```flexscript
closeoutput(obj object)
```

Deprecated, use Object.output.close()

This command is deprecated. Use Object.output.close() instead.

This command blocks the output of the object. Only the output ports themselves, or the output in general needs to be closed to block the exit. When port connections are displayed with sufficient size in the view window, you will see a small red bar drawn across all the output ports (squares) for the object, signifying the output ports are blocked. Individual ports may still be open and drawn in green, but exit from the object is still blocked because of the red bar. Compare this command with stopoutput().

## Example

```flexscript
if (current.subnodes.length == current.mincontent) closeoutput(current);
This blocks the output ports of current, if current has too few objects inside it.
```

