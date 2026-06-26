---
id: select
name: "select"
kind: command
module: ""
signature: "select(obj window) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: select."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# select

```flexscript
select(obj window) -> num
```

For developer use. Select the specified view object. Convert the return value with tonode().

## Example

```flexscript
select(ownerview(c))
```

