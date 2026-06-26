---
id: strascii
name: "strascii"
kind: command
module: ""
signature: "strascii(num asciivalue) -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: strascii."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# strascii

```flexscript
strascii(num asciivalue) -> str
```

This command returns a string that contains the character whose ascii value is character.

## Example

```flexscript
strascii(91) + "alert(" + strascii(34) + "Hello There" + strascii(34) + ")" + strascii(93);
This returns a string that contains the following phrase: [alert("Hello There")]
```

