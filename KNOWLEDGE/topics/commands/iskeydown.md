---
id: iskeydown
name: "iskeydown"
kind: command
module: ""
signature: "iskeydown([num key]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: iskeydown."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# iskeydown

```flexscript
iskeydown([num key]) -> num
```

For developer use. Returns 1 if the specified keyboard key is being held down. If the first parameter is not passed, then the function will return 1 if any key is being held down. Otherwise, the function returns 0.

## Example

```flexscript
iskeydown(74)
```

