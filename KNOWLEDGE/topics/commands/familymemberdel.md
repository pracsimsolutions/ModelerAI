---
id: familymemberdel
name: "familymemberdel"
kind: command
module: ""
signature: "familymemberdel(obj familycontainer, str name, obj member)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "FlexScript command: familymemberdel."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# familymemberdel

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
familymemberdel(obj familycontainer, str name, obj member)
```

This command is deprecated. Use the Group class instead.

Removes a family member from the specified familycontainer.

## Example

```flexscript
familymemberdel(item,"family1",item.subnodes[2])
```

