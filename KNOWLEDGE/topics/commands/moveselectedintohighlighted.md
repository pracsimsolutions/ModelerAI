---
id: moveselectedintohighlighted
name: "moveselectedintohighlighted"
kind: command
module: ""
signature: "moveselectedintohighlighted(obj highlightedobj [, node selectedcontainer])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: moveselectedintohighlighted."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# moveselectedintohighlighted

```flexscript
moveselectedintohighlighted(obj highlightedobj [, node selectedcontainer])
```

For developer use. Moves all selected objects in the model into the highlighted object.

## Example

```flexscript
moveselectedintohighlighted(selectedobject(ownerview(c)))
```

