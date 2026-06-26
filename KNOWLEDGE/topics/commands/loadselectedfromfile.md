---
id: loadselectedfromfile
name: "loadselectedfromfile"
kind: command
module: ""
signature: "loadselectedfromfile(obj highlightedobj)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: loadselectedfromfile."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# loadselectedfromfile

```flexscript
loadselectedfromfile(obj highlightedobj)
```

For developer use. Loads previously saved tree file into the highlighted object.

## Example

```flexscript
loadselectedfromfile(selectedobject(ownerview(c)))
```

