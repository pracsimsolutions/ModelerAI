---
id: clickcode
name: "clickcode"
kind: command
module: ""
signature: "clickcode() -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: clickcode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clickcode

```flexscript
clickcode() -> num
```

For developer use. Returns the type of click in a view. Used in the OnClick event.
DOUBLE_CLICK returns 1 LEFT_PRESS returns 2 LEFT_RELEASE returns 3 RIGHT_PRESS returns 4 RIGHT_RELEASE returns 5

## Example

```flexscript
if(clickcode()==2) { pt("Clicked!!");pr(); }
This prints the statement "Clicked!!" to the output console when the left mouse button is pressed.
```

