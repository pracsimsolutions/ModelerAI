---
id: for
name: "for"
kind: command
module: ""
signature: "for(initalstatement; whiletrueexpression ; countstatement)"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: for."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# for

```flexscript
for(initalstatement; whiletrueexpression ; countstatement)
```

Perform a number of loops. The initialstatement will be executed once before starting the loop. The whiletrueexpression will be evaluated before executing each loop and the loop will stop as soon as this expression is true. The countstatement is executed at the end of each loop and usually increments a counter. Refer to the help manual under Writing Logic in FlexSim for more information.

## Example

```flexscript
for(int i = 1; i <= 5; i++) { msg("for loop",""); }
```

