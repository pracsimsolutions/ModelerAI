---
id: fluidcontent
name: "fluidcontent"
kind: command
module: ""
signature: "fluidcontent(obj fluidobject)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the current fluid content of fluidobject"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fluidcontent

```flexscript
fluidcontent(obj fluidobject)
```

Gets the current fluid content of fluidobject

Returns the current fluid content of the specified Fluid Object. It is the same as reading the "curcontent" variable on the object. This does not have any meaning if a Discrete object is passed in.

## Example

```flexscript
double objcontent = fluidcontent(current.outObjects[1]);
```

