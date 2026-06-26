---
id: stop
name: "stop"
kind: command
module: ""
signature: "stop([num fast])"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Stops the simulation run"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stop

```flexscript
stop([num fast])
```

Stops the simulation run

This command stops the simulation clock from running. The model stops with all of its pending events still in the queue. If the user starts the model again before resetting, the model will continue from the exact time that it stopped at. If fast is 1, then FlexSim will not rebind objects. This will improve speed if you are stopping and starting often.

## Example

```flexscript
stop()
```

