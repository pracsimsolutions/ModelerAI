---
id: fluidmaxcontent
name: "fluidmaxcontent"
kind: command
module: ""
signature: "fluidmaxcontent(obj fluidobject)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the max fluid content of fluidobject"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fluidmaxcontent

```flexscript
fluidmaxcontent(obj fluidobject)
```

Gets the max fluid content of fluidobject

Returns the defined maximum fluid content of the specified Fluid Object. It is the same as reading the "maxcontent" variable on the object. This does not have any meaning if a Discrete object is passed in.

## Example

```flexscript
double objmaxcontent = fluidmaxcontent(current.outObjects[1]);
```

