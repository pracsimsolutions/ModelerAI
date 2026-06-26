---
id: senddelayedmessage
name: "senddelayedmessage"
kind: command
module: ""
signature: "senddelayedmessage(obj toobject, num delaytime, obj fromobject [, num par1, num par2, num par3])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.message"
description: "Send a delayed message to an object, firing its OnMessage trigger after the delay time has transpired"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# senddelayedmessage

> ⚠ **DEPRECATED.** Use `Object.message` instead.

```flexscript
senddelayedmessage(obj toobject, num delaytime, obj fromobject [, num par1, num par2, num par3])
```

Send a delayed message to an object, firing its OnMessage trigger after the delay time has transpired

This command is deprecated. Use Object.message() instead. Causes a message to be sent to the toobject from the fromobject after the delaytime has transpired. When the toobject receives the message, its OnMessage trigger will fire. In the code of the OnMessage trigger, you may refer to the fromobject as msgsendingobject, and the three numeric par values passed in as msgparam(1), msgparam(2) and msgparam(3). This is a very usefuly command for creating your own time events during a model run.

## Example

```flexscript
senddelayedmessage(current.outObjects[1], 25, current, 0, 0, 0);
Sends a message from the current object to the object connecte to output port 1 of the current object 25 time units after this command is executed. Zeros are passed for all three of the user-defined parameters.
```

