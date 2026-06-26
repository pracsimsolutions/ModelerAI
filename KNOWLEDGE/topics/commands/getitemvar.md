---
id: getitemvar
name: "getitemvar"
kind: command
module: ""
signature: "getitemvar(obj item, num var) -> num"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: "this"
description: "Gets the value of a FixedResource variable for the item"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getitemvar

```flexscript
getitemvar(obj item, num var) -> num
```

Gets the value of a FixedResource variable for the item

This returns the value of a FixedResource variable for the item. The var parameter should be either 1, 2, or 3. The FixedResource class allows sub-classes to keep up to 3 number variables on each flowitem. What these variables mean is dependent on the type of class. For example, a Processor uses variable 1 to store the time the item started its processing time, and variable 2 to store the item's processing time. If you are using the explicit FixedResource class as an object in your model, you can use this command, along with the setitemvar, getiteminvolved, and setiteminvolved commands, to store variables on the items the object receives.

## Example

```flexscript
getitemvar(current.first, 3)
```

