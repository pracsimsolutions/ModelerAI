---
id: ipready
name: "ipready"
kind: command
module: ""
signature: "ipready(obj object, num inputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: ipready."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ipready

```flexscript
ipready(obj object, num inputportnum) -> num
```

Returns a 1 if the inputportnum of object is open and the connecting output port of the other object is also open. Otherwise a 0 is returned. This command does not take into consideration whether or not the input of this object or the output of the connecting object are currently open.

## Example

```flexscript
ipready(current,1)
```

