---
id: stringtopchar
name: "stringtopchar"
kind: command
module: ""
signature: "stringtopchar(str string) -> str"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "apchar"
description: "Deprecated, use apchar()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stringtopchar

> ⚠ **DEPRECATED.** Use `apchar` instead.

```flexscript
stringtopchar(str string) -> str
```

Deprecated, use apchar()

This command is deprecated. Use apchar() instead.

This command returns the string as a pointer to a C++ character. This is used to convert variables of type string to type char* for some C++ commands which only take char*'s. It is not needed in Flexscript.

## Example

```flexscript
setname(current,stringtopchar("ThisObjectName"));
This sets the name of the object referenced by current to "ThisObjectName". The stringtopchar() command may not be mandatory here, but it is legal.
```

