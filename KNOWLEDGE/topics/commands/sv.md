---
id: sv
name: "sv"
kind: command
module: ""
signature: "sv() -> node"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Gets a reference to the last view desginated as sv()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# sv

```flexscript
sv() -> node
```

Gets a reference to the last view desginated as sv()

This command returns a reference to the last view window that was designated the selected view. To designate a view window, right-click on the window, not on any objects drawn in that window, and select "Set as (sv)". Any open view window (e.g. Ortho or VR) can be designated the selected view.

## Example

```flexscript
flypath(sv(),1);
This begins flypath number 1 in the view that is currently designated the selected view.
```

