---
id: cmdloadtree
name: "cmdloadtree"
kind: command
module: ""
signature: "cmdloadtree(obj desttree, str filename [, num nobuild])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: cmdloadtree."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cmdloadtree

```flexscript
cmdloadtree(obj desttree, str filename [, num nobuild])
```

For developer use.
Load a tree by file path into desttree.
By default, after loading, all flexscript will be built.
If nobuild is 1, flexscript will not be built. Set nobuild to 1
if you are executing this command from a script node that is toggled
as flexscript, or else your script will not finish properly.

## Example

```flexscript
cmdloadtree(node("MAIN:/project/model/2"),"c:/mytree.t"))
```

