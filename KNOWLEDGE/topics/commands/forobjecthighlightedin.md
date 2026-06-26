---
id: forobjecthighlightedin
name: "forobjecthighlightedin"
kind: command
module: ""
signature: "forobjecthighlightedin(obj view) { }"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "selectedobject"
description: "FlexScript command: forobjecthighlightedin."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# forobjecthighlightedin

> ⚠ **DEPRECATED.** Use `selectedobject` instead.

```flexscript
forobjecthighlightedin(obj view) { }
```

This command is deprecated. Use selectedobject() instead.

Iterate on the node that is both selected (red) and highlighted (yellow) in the selected view. Reference the selected and highlighted node by using "a" within the loop.

## Example

```flexscript
forobjecthighlightedin(sv()) { pt(a.name); pr(); }
This prints to the outputconsole the name of the selected, highlighted node in the first level of the model.
```

