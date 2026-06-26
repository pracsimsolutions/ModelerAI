---
id: getproperty
name: "getproperty"
kind: command
module: ""
signature: "getproperty(str filename, num property)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: getproperty."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getproperty

```flexscript
getproperty(str filename, num property)
```

For developer use. Gets the value for the specified property in the specified file.

## Example

```flexscript
getproperty(modeldir() + "MyModel.fsm",1);
```

