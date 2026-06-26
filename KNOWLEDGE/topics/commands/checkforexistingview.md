---
id: checkforexistingview
name: "checkforexistingview"
kind: command
module: ""
signature: "checkforexistingview(obj view [, num mode]) -> node"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: checkforexistingview."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# checkforexistingview

```flexscript
checkforexistingview(obj view [, num mode]) -> node
```

For developer use. Checks to see if there are any currently active windows that match this window. Compares the beginning of each name of each window in the active view node with the beginning of the name of the specified view. Along with comparing the beginning of the names, this compares each objectfocus as well. If mode is specified and is not 0, it will compare the viewfocus instead of the objectfocus.

## Example

```flexscript
checkforexistingview(c)
```

