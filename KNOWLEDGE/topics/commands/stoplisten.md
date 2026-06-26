---
id: stoplisten
name: "stoplisten"
kind: command
module: ""
signature: "stoplisten(obj listener)"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: stoplisten."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stoplisten

```flexscript
stoplisten(obj listener)
```

For developer use. Tell the executive that this object is not a listener anymore. This command will remove the pointer to listener from the listeners list if previously added with the startlisten() command. Listeners will receive OnListen messages when an event (OnTimerEvent) is dispatched to any object in the model.

## Example

```flexscript
stoplisten(so())
```

