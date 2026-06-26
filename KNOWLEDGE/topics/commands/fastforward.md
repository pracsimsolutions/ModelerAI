---
id: fastforward
name: "fastforward"
kind: command
module: ""
signature: "fastforward()"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "FlexScript command: fastforward."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fastforward

```flexscript
fastforward()
```

For developer use. Saves off the current runspeed, sets the run speed to max, suspends draw, and then runs to the next stop time. Once the next stop time is reached drawing is resumed and the runspeed is restored.

## Example

```flexscript
fastforward()
```

