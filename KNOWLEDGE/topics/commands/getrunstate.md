---
id: getrunstate
name: "getrunstate"
kind: command
module: ""
signature: "getrunstate([int checkProcessingEvent]) -> num"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Returns 1 if the model is currently running, 0 if not"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getrunstate

```flexscript
getrunstate([int checkProcessingEvent]) -> num
```

Returns 1 if the model is currently running, 0 if not

Returns 1 if the model is currently running, 0 if not. If you pass a true value, returns whether an event is currently executing.

## Example

```flexscript
if(!getrunstate()){msg("Model Stopped", "The model is not running right now.",1);}
If executed from the script window,
this message will only appear if the model is stopped.
```

