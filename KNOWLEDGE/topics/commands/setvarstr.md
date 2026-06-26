---
id: setvarstr
name: "setvarstr"
kind: command
module: ""
signature: "setvarstr(obj object, str/num var, str value)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Sets the value of an object's string variable"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setvarstr

```flexscript
setvarstr(obj object, str/num var, str value)
```

Sets the value of an object's string variable

Set the string value of the specified variable on the object.

## Example

```flexscript
setvarstr(current,"cycletime","return exponential(10,1);");
```

