---
id: opready
name: "opready"
kind: command
module: ""
signature: "opready(obj object, num outputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: opready."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# opready

```flexscript
opready(obj object, num outputportnum) -> num
```

Returns a 1 if the outputportnum of object is open and the connecting input port of the other object is also open. Otherwise a 0 is returned. This command does not take into consideration whether or not the output of this object or the input of the connecting object are currently open.

## Example

```flexscript
opready(current,1)
```

