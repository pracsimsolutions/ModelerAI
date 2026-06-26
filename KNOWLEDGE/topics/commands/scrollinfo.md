---
id: scrollinfo
name: "scrollinfo"
kind: command
module: ""
signature: "scrollinfo(obj/HWND view, num get_set {0-1}, num horz_vert_ctl {0-2}, num scrollinfo {1-4}[, num value]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: scrollinfo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# scrollinfo

```flexscript
scrollinfo(obj/HWND view, num get_set {0-1}, num horz_vert_ctl {0-2}, num scrollinfo {1-4}[, num value]) -> num
```

For developer use. Gets or sets scrollinfo for a scrollbar on a view. The value parameter is only used if get_set is 1. The position of the scrollbar (nPos) is changed with scrollinfo 4. You can also get and set the nMin, nMax, and nPage values of the scrollbar with scrollinfo 1, 2, and 3 respectively.

## Example

```flexscript
scrollinfo(TheTable,1,1,4,scrollinfo(c,0,1,4)-amount);
```

