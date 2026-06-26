---
id: swapnoderanks
name: "swapnoderanks"
kind: command
module: ""
signature: "swapnoderanks(obj/node parentnode, num index1, num index2)"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Swap the ranks for two nodes in a parent node's sub-tree"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# swapnoderanks

```flexscript
swapnoderanks(obj/node parentnode, num index1, num index2)
```

Swap the ranks for two nodes in a parent node's sub-tree

This command swaps the ranks of two nodes in the parent node's sub-tree. Swapping ranks can be faster than using setrank() especially for very large lists of nodes.

## Example

```flexscript
swapnoderanks(parentNode,4,parentNode.subnodes.length);
This swaps the 4th ranked node in parentNode's sub-tree with the last node in parentNode's sub-tree. This can make removing that node from parentNode's sub-tree faster if the list is very long. Note that this will put the last node into the 4th position, so ordering in the tree should not matter to be able to do this. In other words, this would break fifo ordering.
```

