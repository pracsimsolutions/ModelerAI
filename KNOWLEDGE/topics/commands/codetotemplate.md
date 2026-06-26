---
id: codetotemplate
name: "codetotemplate"
kind: command
module: ""
signature: "codetotemplate(obj code, obj edit)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: codetotemplate."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# codetotemplate

```flexscript
codetotemplate(obj code, obj edit)
```

For developer use.
Converts C++/Flexscript code into template code and sets the view text of edit to the template code. The code parameter should reference a node that contains text with C++/Flexscript code on it. This is used in a picklist's template code drop-down to create template code from the actual C++/Flexscript code.

## Example

```flexscript
codetotemplate(so(),sv())
```

