---
id: setvarnum
name: "setvarnum"
kind: command
module: ""
signature: "setvarnum(obj object, str/num var, num value)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Sets the value of an object's numeric variable"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setvarnum

```flexscript
setvarnum(obj object, str/num var, num value)
```

Sets the value of an object's numeric variable

Set the specified variable on the object to the number passed in as value.

## Example

```flexscript
setvarnum(current.centerObjects[1],"speed",1.5);
```

