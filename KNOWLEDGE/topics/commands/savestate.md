---
id: savestate
name: "savestate"
kind: command
module: ""
signature: "savestate()"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: savestate."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# savestate

```flexscript
savestate()
```

For developer use. Saves the state of the project. Once this command is executed, you can save your project (.fsp), then open in later, compile, execute loadstate(), and run from the previous spot you started from.

## Example

```flexscript
savestate();
```

