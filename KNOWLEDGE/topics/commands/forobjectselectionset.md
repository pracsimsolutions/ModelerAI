---
id: forobjectselectionset
name: "forobjectselectionset"
kind: command
module: ""
signature: "forobjectselectionset(node container) { }"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: forobjectselectionset."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# forobjectselectionset

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
forobjectselectionset(node container) { }
```

This command is deprecated. Instead use:
for(int i =1; i<= container.subnodes.length; i++) { if(switch_selected(container.subnodes[i])) { pt(container.subnodes[i].name); pr(); } }

The block following the command will be iterated upon once for each selected object in the container. The value of the "a" command will return the pointer to the object being iterated upon. This command is not available in C++.

## Example

```flexscript
forobjectselectionset(library()) { pt(a.name); pr(); }
This prints to the outputconsole the name of every object currently selected in the library tree.
```

