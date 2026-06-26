---
id: getsystemmetric
name: "getsystemmetric"
kind: command
module: ""
signature: "getsystemmetric(num metric[, HWND win]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: getsystemmetric."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getsystemmetric

```flexscript
getsystemmetric(num metric[, HWND win]) -> num
```

For developer use. Returns system screen metrics. These values for metric return the following values (if not specified, values are in logical units):

1: desktop width
2: desktop height
3: the left position of the work area of the monitor under the cursor
4: the right position of the work area of the monitor under the cursor
5: the top position of the work area of the monitor under the cursor
6: the bottom position of the work area of the monitor under the cursor
7: the width of window win in pixels
8: the height of window win in pixels
9: the left position of the monitor under the cursor
10: the right position of the monitor under the cursor
11: the top position of the monitor under the cursor
12: the bottom position of the monitor under the cursor
13: the width of window win
14: the height of window win

## Example

```flexscript
getsystemmetric(1)
```

