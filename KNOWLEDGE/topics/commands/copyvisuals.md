---
id: copyvisuals
name: "copyvisuals"
kind: command
module: ""
signature: "copyvisuals(obj highlightedobj [, num copyshapefactorsalso])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: copyvisuals."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# copyvisuals

```flexscript
copyvisuals(obj highlightedobj [, num copyshapefactorsalso])
```

For developer use. Copies all visuals from the parameter 1 object to all selected objects in the model.

## Example

```flexscript
copyvisuals(selectedobject(ownerview(c)),1)
```

