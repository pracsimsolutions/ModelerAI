---
id: msgsendingobject
name: "msgsendingobject"
kind: command
module: ""
signature: "msgsendingobject() -> node"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "Within an OnMessage trigger, get a reference to the object sending the message"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# msgsendingobject

```flexscript
msgsendingobject() -> node
```

Within an OnMessage trigger, get a reference to the object sending the message

Returns a reference to the object that sent the message as defined in the Object.message() method or sendmessage() or senddelayedmessage() command. Only used in OnMessage triggesr.

## Example

```flexscript
treenode sender = msgsendingobject;
```

