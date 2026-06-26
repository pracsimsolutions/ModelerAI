---
id: copylabels
name: "copylabels"
kind: command
module: ""
signature: "copylabels(obj highlightedobj[, num mode])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: copylabels."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# copylabels

```flexscript
copylabels(obj highlightedobj[, num mode])
```

For developer use. Copies all labels from the parameter 1 object to all selected objects in the model. By default it clears out all existing
labels and replaces them with the labels on the parameter 1 object. If mode is 1, existing labels will be overwritten by any incoming labels with the same name,
but will otherwise be left alone. If mode is 2, only labels that the selected objects do not already have will be copied.

## Example

```flexscript
copylabels(selectedobject(ownerview(c)), c.find(">labelname"))
```

