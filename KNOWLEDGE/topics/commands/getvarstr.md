---
id: getvarstr
name: "getvarstr"
kind: command
module: ""
signature: "getvarstr(obj object, str/num var) -> str"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the value of an object's string variable"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getvarstr

```flexscript
getvarstr(obj object, str/num var) -> str
```

Gets the value of an object's string variable

Returns the string data stored on the specified variable of the specified object.

## Example

```flexscript
pt(getvarstr(model().subnodes[2],"creationtrigger"));
```

