---
id: setprogressbar
name: "setprogressbar"
kind: command
module: ""
signature: "setprogressbar(num progress)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: setprogressbar."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setprogressbar

```flexscript
setprogressbar(num progress)
```

For developer use. Sets the progress bar to the specified value. Should be a value between 0 and 100.

## Example

```flexscript
setprogressbar((numread / numtotal) * 100);
```

