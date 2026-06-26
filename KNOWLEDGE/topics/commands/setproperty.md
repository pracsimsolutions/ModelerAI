---
id: setproperty
name: "setproperty"
kind: command
module: ""
signature: "setproperty(str filename, num property, str newvalue)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: setproperty."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setproperty

```flexscript
setproperty(str filename, num property, str newvalue)
```

For developer use. Sets the value for the specified property in the specified file to the newvalue.

## Example

```flexscript
setproperty(modeldir() + "MyModel.fsm",1,"Custom Title");
```

