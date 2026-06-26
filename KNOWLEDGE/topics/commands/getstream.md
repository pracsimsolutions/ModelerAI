---
id: getstream
name: "getstream"
kind: command
module: ""
signature: "getstream(node object) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "Returns a unique random stream associated with the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstream

```flexscript
getstream(node object) -> num
```

Returns a unique random stream associated with the object

Returns a unique random stream associated with the object.
Returns the number stored in the object's stream attribute. If the object does not yet
own a stream attribute, or if its stream attribute is 0, FlexSim will assert the attribute and assign it a unique stream number.
If the object passed into the function does not have object data, it will store the unique
stream in a subnode. This subnode will be destroyed on model reset and reassigned during the model run as needed.

Assigned streams start at stream 101 and increase as they are assigned.

## Example

```flexscript
uniform(0, 1, getstream(current))
```

