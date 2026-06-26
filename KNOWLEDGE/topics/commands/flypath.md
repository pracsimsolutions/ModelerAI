---
id: flypath
name: "flypath"
kind: command
module: ""
signature: "flypath(obj view, num pathno [, num singlerun])"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: flypath."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# flypath

```flexscript
flypath(obj view, num pathno [, num singlerun])
```

For developer use. This command begins the predefined flypath numbered pathno in the view designated specified as view. Any view window can be passed as view.

## Example

```flexscript
flypath(ownerview(c),1);
This will run flypath number 1 in the view window containing an event's code.
```

