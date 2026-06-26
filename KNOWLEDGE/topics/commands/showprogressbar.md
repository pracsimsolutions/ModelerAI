---
id: showprogressbar
name: "showprogressbar"
kind: command
module: ""
signature: "showprogressbar(str caption)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: "setprogressbar"
description: "FlexScript command: showprogressbar."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# showprogressbar

```flexscript
showprogressbar(str caption)
```

For developer use. Opens the progress bar with the specified caption. The bar's progress will be initialized to 0. Use setprogressbar to update the progess value.

## Example

```flexscript
showprogressbar("Loading data, please wait");
```

