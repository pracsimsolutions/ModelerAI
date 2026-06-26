---
id: loadstate
name: "loadstate"
kind: command
module: ""
signature: "loadstate()"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: loadstate."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loadstate

```flexscript
loadstate()
```

For developer use. Loads the state of the project. If savestate() has been executed, then this command must be executed before you can run the model again.

## Example

```flexscript
loadstate();
```

