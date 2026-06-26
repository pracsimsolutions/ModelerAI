---
id: setitemtype
name: "setitemtype"
kind: command
module: ""
signature: "setitemtype(obj object, num value)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "labels"
description: "Deprecated, use labels"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setitemtype

> ⚠ **DEPRECATED.** Use `labels` instead.

```flexscript
setitemtype(obj object, num value)
```

Deprecated, use labels

This command is deprecated. Use labels instead.

This command sets the itemtype attribute of the object to value. If the object does not have the itemtype attribute, this command does nothing. The attribute itemtype is typically used to distinguish between different types of flowitems that may be of the same class.

## Example

```flexscript
setitemtype(item,duniform(1,3));
This sets the itemtype attribute of the flowitem referenced by item to a random integer between 1 and 3.
```

