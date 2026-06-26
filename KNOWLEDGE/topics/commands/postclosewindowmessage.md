---
id: postclosewindowmessage
name: "postclosewindowmessage"
kind: command
module: ""
signature: "postclosewindowmessage(obj view)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "Posts a Windows Message to close the window"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# postclosewindowmessage

```flexscript
postclosewindowmessage(obj view)
```

Posts a Windows Message to close the window

Posts a message onto the Windows message queue to close the window. This allows you to finish any scripting code before the window is actually closed.

## Example

```flexscript
postclosewindowmessage(ownerview(c));
```

