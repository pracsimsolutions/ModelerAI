---
id: drawfont
name: "drawfont"
kind: command
module: ""
signature: "drawfont(num font)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Specifies the font to be used with drawtext()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawfont

```flexscript
drawfont(num font)
```

Specifies the font to be used with drawtext()

Call before drawtext, to define font to be drawn. After drawtext is called, the font will be reset to default. There are currently 12 font types (numbers 0 through 11).

## Example

```flexscript
drawfont(2)
```

