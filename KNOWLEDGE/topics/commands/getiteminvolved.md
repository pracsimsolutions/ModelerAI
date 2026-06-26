---
id: getiteminvolved
name: "getiteminvolved"
kind: command
module: ""
signature: "getiteminvolved(obj item) -> node"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: "operators"
description: "Gets the object reference stored in the FixedResource involved variable for the item"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getiteminvolved

```flexscript
getiteminvolved(obj item) -> node
```

Gets the object reference stored in the FixedResource involved variable for the item

This returns the object reference stored in the FixedResource involved variable for the item. The FixedResource class allows sub-classes to store 1 involved variable on each flowitem. What these variables mean is dependent on the type of class. For example, a Processor uses the involved variable to store a reference to the dispatcher called for a flowitem if the Processor is set to use operators for the process or setup time. If you are using the explicit FixedResource class as an object in your model, you can use this command, along with the setitemvar, getitemvar, and setiteminvolved commands, to store variables on the items the object receives.

## Example

```flexscript
getiteminvolved(current.first)
```

