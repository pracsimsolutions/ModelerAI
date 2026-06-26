---
id: return
name: "return"
kind: command
module: ""
signature: "return(num value)"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: return."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# return

```flexscript
return(num value)
```

This command causes a function to stop executing and return value to the function that called it.

## Example

```flexscript
double res = item.rank * 5; return res;
This sets the value of res to 5 times the rank of the item referenced by item. The value stored in res is then returned to the function that called this one.
```

