---
id: createcopy
name: "createcopy"
kind: command
module: ""
signature: "createcopy(node thenode, node container [, num samename, num inobject, num cached, num replace]) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "treenode.copy"
description: "Copies thenode into the specified container"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createcopy

> ⚠ **DEPRECATED.** Use `treenode.copy` instead.

```flexscript
createcopy(node thenode, node container [, num samename, num inobject, num cached, num replace]) -> node
```

Copies thenode into the specified container

This command is deprecated. Use treenode.copy() instead.

Copy thenode into the node specified as container. This command is different from createinstance() in that it does not attempt to establish any links between thenode and any class. If an instantiation is required, use createinstance() instead.

If samename is 1, the copy will have the same name as thenode.

If samename is 0, the newly created node's name will be thenode's name with a random number added to the end.

If inobject is 1, the copy will be created in the object data portion of the container. If the container does not have object data, the command does nothing.

If cached is 1, a pre-cached version of the node will be copied. This is generally faster.

If replace is 1, the copy will overwrite container, instead of being placed inside it.

If those parameters are not specified, they default to 0.

## Example

```flexscript
createcopy(Model.find("Tools/FlowItemBin/7/1"), current)
```

