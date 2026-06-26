---
id: dellabel
name: "dellabel"
kind: command
module: ""
signature: "dellabel(obj object, str/num label)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "treenode.labels"
description: "Deprecated, use treenode.labels[\"LabelName\"].destroy()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dellabel

> ⚠ **DEPRECATED.** Use `treenode.labels` instead.

```flexscript
dellabel(obj object, str/num label)
```

Deprecated, use treenode.labels["LabelName"].destroy()

This command is deprecated. Use treenode.labels["LableName"].destroy() instead.

Deletes the specified label from the node called "labels" on the specified object. The specified label can either be the labelname as a string or the label's rank as a number. If the label does not exist in that node, this command does nothing.

## Example

```flexscript
dellabel(current,"MyLabel")
```

