---
id: windowflash
name: "windowflash"
kind: command
module: ""
signature: "windowflash(HWND window, num count, num timeout)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: windowflash."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# windowflash

```flexscript
windowflash(HWND window, num count, num timeout)
```

For developer use. Causes the specified window to flash. Count is the number of times to flash the window. Timeout controls the time between flashes. If timeout is 0, the default cursor blink rate is used.

## Example

```flexscript
windowflash(windowfromnode(ownerview(c)),2,125);
```

