---
id: highlightviewtext
name: "highlightviewtext"
kind: command
module: ""
signature: "highlightviewtext(node view, num pos, num span, num mode span/lines)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Highlights text in a code edit window"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# highlightviewtext

```flexscript
highlightviewtext(node view, num pos, num span, num mode span/lines)
```

Highlights text in a code edit window

Highlights text in a scintilla code edit window. This is used by the flexscript syntax checker to go to highlight a syntax error in the code edit window. You can either highlight whole lines or individual characters. To highlight individual characters, specify mode as 2. For lines specify mode as 1. For lines, specify pos as the start line to highlight(the first line is line 1) and span the number of lines to highlight. For individual characters, specify pos as the start position and span as the number of characters to highlight (position 0 is the position to the left to the first character)

## Example

```flexscript
highlightviewtext(c, 0, 5, 2)
This will highlight the first 5 characters of the scintilla text edit.
highlightviewtext(c, 1, 1, 1) This will highlight the first line of the scintilla text edit.
```

