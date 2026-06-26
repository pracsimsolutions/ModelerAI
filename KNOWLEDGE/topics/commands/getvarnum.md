---
id: getvarnum
name: "getvarnum"
kind: command
module: ""
signature: "getvarnum(obj object, str/num var) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the value of an object's numeric variable"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getvarnum

```flexscript
getvarnum(obj object, str/num var) -> num
```

Gets the value of an object's numeric variable

Returns the numeric value stored in the specified variable of the specified object.

## Example

```flexscript
double speed = getvarnum(current,"speed");
```

