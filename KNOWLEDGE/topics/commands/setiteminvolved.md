---
id: setiteminvolved
name: "setiteminvolved"
kind: command
module: ""
signature: "setiteminvolved(obj item, obj involved)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: "operators"
description: "FlexScript command: setiteminvolved."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setiteminvolved

```flexscript
setiteminvolved(obj item, obj involved)
```

This sets the object reference stored in the FixedResource involved variable for the item. The FixedResource class allows sub-classes to store 1 involved variable on each flowitem. What these variables mean is dependent on the type of class. For example, a Processor uses the involved variable to store a reference to the dispatcher called for a flowitem if the Processor is set to use operators for the process or setup time. Only use this command if you are using the explicit FixedResource class as and object in your model. You can use this command, along with the setitemvar, getitemvar, and getiteminvolved commands, to store variables on the items the object receives.

## Example

```flexscript
setiteminvolved(current.first, current.centerObjects[1])
```

