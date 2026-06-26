---
id: rebindobjectattributes
name: "rebindobjectattributes"
kind: command
module: ""
signature: "rebindobjectattributes(node thenode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "Rebinds object attributes"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rebindobjectattributes

```flexscript
rebindobjectattributes(node thenode)
```

Rebinds object attributes

Rebinds object attributes for the given node. If you delete or add an attribute to an object, the object's attribute cache will not immediately reflect that change, so you can call rebindobjectattributes to update the object's attribute cache. This is the same as right-clicking on the object and selecting Edit|Rebind Object Attributes.

## Example

```flexscript
rebindobjectattributes(c)
```

