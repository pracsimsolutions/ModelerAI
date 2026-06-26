---
id: savebyname
name: "savebyname"
kind: command
module: ""
signature: "savebyname(obj tree, str filename)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: savebyname."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# savebyname

```flexscript
savebyname(obj tree, str filename)
```

For developer use. Saves the specified tree node to a file with the given name.

## Example

```flexscript
savebyname(Model.find("MyProcessor"), "C:\\MyProcessor.t");
```

