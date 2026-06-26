---
id: pickedcolorblue
name: "pickedcolorblue"
kind: command
module: ""
signature: "pickedcolorblue() -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickedcolorblue."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickedcolorblue

```flexscript
pickedcolorblue() -> num
```

For developer use. Returns the blue component of a color picked with the pickcolor() command.

## Example

```flexscript
pickcolor(); int bluecomponent = pickedcolorblue();
The bluecomponent variable will be set to the rgb value for the blue component of the color chosen with pickcolor().
```

