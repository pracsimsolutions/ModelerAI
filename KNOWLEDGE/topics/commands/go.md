---
id: go
name: "go"
kind: command
module: ""
signature: "go([num fast])"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Starts the simulation run"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# go

```flexscript
go([num fast])
```

Starts the simulation run

This command starts the simulation run. If there are any events in the event queue, they will occur when the clock reaches them. If there are no events in the queue, the clock will stop. This command will not reset the simulation. If fast is 1, then it will not bind objects before starting. This improves speed if you are stopping and starting the simulation often. See also runfunction().

## Example

```flexscript
stop(); if (msg("Model Stopped","Continue?")==1) { go(); }
This stops the model then prompts the user if it should start again. If the user presses "OK", the model continues from where it was stopped.
```

