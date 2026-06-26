---
id: optimizenetwork
name: "optimizenetwork"
kind: command
module: ""
signature: "optimizenetwork()"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: "this"
description: "Causes the model's network navigator to regenerate its distance table"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# optimizenetwork

```flexscript
optimizenetwork()
```

Causes the model's network navigator to regenerate its distance table

Causes the model's network navigator to regenerate its distance table. Use this command if you are dynamically connecting edges in your network during the simulation run.This may take several seconds to do for large networks.

## Example

```flexscript
optimizenetwork()
```

