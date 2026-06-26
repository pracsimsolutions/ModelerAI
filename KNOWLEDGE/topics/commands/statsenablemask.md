---
id: statsenablemask
name: "statsenablemask"
kind: command
module: ""
signature: "statsenablemask(num mask)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: statsenablemask."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# statsenablemask

```flexscript
statsenablemask(num mask)
```

For developer use. Enable or disable aspects of stats collection. Set a 32 bit enable mask: Sum of flags. Bits: 0 = State; 1 = Flow; 2 = Inside Ports.

## Example

```flexscript
statsenablemask(add(1,2))
```

