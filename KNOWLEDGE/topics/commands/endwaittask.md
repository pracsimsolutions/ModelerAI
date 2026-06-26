---
id: endwaittask
name: "endwaittask"
kind: command
module: ""
signature: "endwaittask(obj taskexecuter)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow or freeoperators()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# endwaittask

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
endwaittask(obj taskexecuter)
```

Deprecated, use Process Flow or freeoperators()

This command is deprecated. Use Process Flow instead or the freeoperators() command instead.

This command releases the object specified as dispatcher from a wait task issued by the createwaittask(). Once the object is released, it will return to the state that it was previously in, and it will begin to perform any task sequences that are in its tasksequence queue.

## Example

```flexscript
endwaittask(current.centerObjects[1]);
```

