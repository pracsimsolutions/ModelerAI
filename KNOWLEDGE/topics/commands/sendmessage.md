---
id: sendmessage
name: "sendmessage"
kind: command
module: ""
signature: "sendmessage(obj toobject, obj fromobject [, var par1, var par2, var par3])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.message"
description: "Send a message to an object, firing its OnMessage trigger"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# sendmessage

> ⚠ **DEPRECATED.** Use `Object.message` instead.

```flexscript
sendmessage(obj toobject, obj fromobject [, var par1, var par2, var par3])
```

Send a message to an object, firing its OnMessage trigger

This command is deprecated. Use Object.message() instead. Causes a message to be sent immediately to the toobject from the fromobject as in a direct function call. When the toobject receives the message, its OnMessage trigger will fire. In the code of the OnMessage trigger, you may refer to the fromobject as msgsendingobject, and the three par values passed in as msgparam(1), msgparam(2) and msgparam(3).
These message parameters are Variants, meaning they can be numbers, strings, treenode references or arrays.

## Example

```flexscript
sendmessage(current.centerObjects[1], current, item, 10);
This sends a message to the object connected to current's first center port. A reference to item is passed as par1 and the number 10 as par2.
```

