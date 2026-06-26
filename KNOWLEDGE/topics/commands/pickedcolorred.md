---
id: pickedcolorred
name: "pickedcolorred"
kind: command
module: ""
signature: "pickedcolorred() -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickedcolorred."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickedcolorred

```flexscript
pickedcolorred() -> num
```

For developer use. Returns the red component of a color picked with the pickcolor() command.

## Example

```flexscript
pickcolor(); int redcomponent = pickedcolorred();
The redcomponent variable will be set to the rgb value for the red component of the color chosen with pickcolor().
```

