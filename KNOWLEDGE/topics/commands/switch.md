---
id: switch
name: "switch"
kind: command
module: ""
signature: "switch(num value)"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch

```flexscript
switch(num value)
```

Execute one of several possible cases, based on value

## Example

```flexscript
int msgcase = msgparam(1);switch(msgcase) { case 1:msg("","case 1");break;case 2:msg("","case 2");break;default:msg("","case 2");break; }
```

