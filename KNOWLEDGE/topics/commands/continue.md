---
id: continue
name: "continue"
kind: command
module: ""
signature: "continue()"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: continue."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# continue

```flexscript
continue()
```

Continues immediately to the next iteration of a for or while loop without executing any of the remaining code in the current loop.

## Example

```flexscript
if(thisloopnotneeded) continue;
```

