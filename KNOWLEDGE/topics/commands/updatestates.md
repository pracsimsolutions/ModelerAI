---
id: updatestates
name: "updatestates"
kind: command
module: ""
signature: "updatestates()"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Updates the states of all objects in the model"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# updatestates

```flexscript
updatestates()
```

Updates the states of all objects in the model

Updates the states of all objects in the model. Additionally, this command also updates the content stats of all the objects. The 'Stop' button and menu option both call this function before executing a stop() so that objects' stats are updated. Otherwise, reports will not reflect the time the object was in its last state.

## Example

```flexscript
updatestates();
```

