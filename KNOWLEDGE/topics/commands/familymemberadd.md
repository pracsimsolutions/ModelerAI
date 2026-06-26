---
id: familymemberadd
name: "familymemberadd"
kind: command
module: ""
signature: "familymemberadd(obj familycontainer, str name, obj member)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "FlexScript command: familymemberadd."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# familymemberadd

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
familymemberadd(obj familycontainer, str name, obj member)
```

This command is deprecated. Use the Group class instead.

Adds a new family member to the specified familycontainer.

## Example

```flexscript
familymemberadd(item,"family1",item.subnodes[1])
```

