---
id: pickedcolorgreen
name: "pickedcolorgreen"
kind: command
module: ""
signature: "pickedcolorgreen() -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickedcolorgreen."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickedcolorgreen

```flexscript
pickedcolorgreen() -> num
```

For developer use. Returns the green component of a color picked with the pickcolor() command.

## Example

```flexscript
pickcolor(); int greencomponent = pickedcolorgreen();
The greencomponent variable will be set to the rgb value for the green component of the color chosen with pickcolor().
```

