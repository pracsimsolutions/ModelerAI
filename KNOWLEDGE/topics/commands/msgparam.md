---
id: msgparam
name: "msgparam"
kind: command
module: ""
signature: "msgparam(num param) -> var"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "Within an OnMessage trigger, get the value of a message parameter"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# msgparam

```flexscript
msgparam(num param) -> var
```

Within an OnMessage trigger, get the value of a message parameter

Returns a parameter passed to with the Object.message() method or sendmessage() or senddelayedmessage() commands. Only used in OnMessage triggers.

## Example

```flexscript
int msgtype = msgparam(1);
treenode item = msgparam(2);
Array data = msgparam(3);
```

