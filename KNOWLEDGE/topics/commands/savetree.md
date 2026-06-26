---
id: savetree
name: "savetree"
kind: command
module: ""
signature: "savetree(node tree, str extension, str description, str startingpath)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: savetree."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# savetree

```flexscript
savetree(node tree, str extension, str description, str startingpath)
```

For developer use. Opens a save file dialog prompting the user for a file name to save. The tree passed in will be saved with the specified name, unless a session is selected. Then the first parameter is ignored. cdir() and pdir() may not be directly used as the path in FlexScript.

## Example

```flexscript
savetree(model(),"fsm","FlexSim Model",documentsdir());
```

