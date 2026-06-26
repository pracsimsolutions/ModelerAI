---
id: familymember
name: "familymember"
kind: command
module: ""
signature: "familymember(obj familycontainer, str name, num index) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "FlexScript command: familymember."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# familymember

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
familymember(obj familycontainer, str name, num index) -> node
```

This command is deprecated. Use the Group class instead.

Returns a pointer to a member of the specified family.

## Example

```flexscript
familymember(item,"family1",2)
```

