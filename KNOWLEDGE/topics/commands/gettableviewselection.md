---
id: gettableviewselection
name: "gettableviewselection"
kind: command
module: ""
signature: "gettableviewselection(obj tableview, num val) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: gettableviewselection."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettableviewselection

```flexscript
gettableviewselection(obj tableview, num val) -> num
```

For developer use. Returns values based on the blue selection range in a table view: val 1 returns the start row, val 2 returns the start col, val 3 returns the end row, val 4 returns the end col.

## Example

```flexscript
int startrow = gettableviewselection(c,1);
```

