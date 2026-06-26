---
id: applylinks
name: "applylinks"
kind: command
module: ""
signature: "applylinks(node view [, num refresh])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: applylinks."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# applylinks

```flexscript
applylinks(node view [, num refresh])
```

For developer use. This command applies all of the hotlinks and coldlinks in the view window specified as view. If refresh is 1, then the links will be refreshed, meaning the values from the model will be retrieved and set on the window controls themselves. If refresh is 0, then the values in the model will be set according to what the user specified in the view. This command is used in Flexscript GUI programming.

## Example

```flexscript
applylinks(ownerview(c));
This applies all of the links in the window that is currently designated as the selected view.
```

