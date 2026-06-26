---
id: isclasstype
name: "isclasstype"
kind: command
module: ""
signature: "isclasstype(obj object, num/str classtype) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Compares the object to classtype, returns 1 for yes, 0 for no"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# isclasstype

```flexscript
isclasstype(obj object, num/str classtype) -> num
```

Compares the object to classtype, returns 1 for yes, 0 for no

Returns whether or not the object is of the given classtype. 1 means yes, 0 means no. Possible classtype parameters are:CLASSTYPE_FLEXSIMOBJECT CLASSTYPE_FIXEDRESOURCE CLASSTYPE_TASKEXECUTER CLASSTYPE_NETWORKNODEetc.

## Example

```flexscript
isclasstype(current, CLASSTYPE_DISPATCHER);
```

