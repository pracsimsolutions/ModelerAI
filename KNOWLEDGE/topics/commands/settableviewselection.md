---
id: settableviewselection
name: "settableviewselection"
kind: command
module: ""
signature: "settableviewselection(obj tableview, num startrow, num startcol, num endrow, num endcol[, num rangeselected])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: settableviewselection."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# settableviewselection

```flexscript
settableviewselection(obj tableview, num startrow, num startcol, num endrow, num endcol[, num rangeselected])
```

For developer use. Sets the blue selection range in a table view. If any of the number parameters are less than 0, it deselects everything. Use 0 to select header cells.

## Example

```flexscript
settableviewselection(c,0,1,2,2);
```

