---
id: transitionport
name: "transitionport"
kind: command
module: ""
signature: "transitionport() -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: transitionport."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# transitionport

```flexscript
transitionport() -> num
```

Get the most recently traversed port number by an object in the model. It refers to the last moveobject() call which occurred in the model run.

## Example

```flexscript
transitionport()
```

