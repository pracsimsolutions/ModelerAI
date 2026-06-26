---
id: startlisten
name: "startlisten"
kind: command
module: ""
signature: "startlisten(obj listener)"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: "minimally."
description: "FlexScript command: startlisten."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# startlisten

```flexscript
startlisten(obj listener)
```

For developer use. Tell the executive that an object is a listener. Not persistent across sessions. This command will add a node in the executive listener list pointing to listener. When a session is reloaded, or after a compile, listeners need to be reset. The listening mechanism can add a significant load to the executive, so use minimally. Listeners will receive OnListen messages when an event (OnTimerEvent) is dispatched to any object in the model. The listener must filter the message to decide whether to respond to the OnListen message. Refer to the Online Reference Guide for the parameters passed into an OnListen message.

## Example

```flexscript
startlisten(so())
```

