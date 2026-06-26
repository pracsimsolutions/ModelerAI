---
id: eventlisten
name: "eventlisten"
kind: command
module: ""
signature: "eventlisten(obj object, str eventName, node relayNode[, num flags, par1, par2, ... par10]) -> node"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: eventlisten."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# eventlisten

```flexscript
eventlisten(obj object, str eventName, node relayNode[, num flags, par1, par2, ... par10]) -> node
```

This command will create a listening mechanism where whenever the defined event is called on object, relayNode will also be executed.
flags is a bitwise mask of one of the following values:
EVENT_PRE_LISTEN: The listening function will be called before theNode is called.
EVENT_LISTEN: (default) The listening function will be called after theNode is called.
EVENT_PERSIST: The listener will persist across model resets.
EVENT_PASS_THROUGH_PARAMS: When theNode is called, the parameters passed into the event will also be "passed through"
into the listening node. In this case, par1, par2, etc. are not used.
EVENT_LISTEN_ONCE: The listening mechanism will automatically be removed after the first event firing.
When the listening node is called, if it returns EVENT_STOP_LISTENING, the listener mechanism will be removed and the listening node will no longer be called.
The return value is a reference to a node associated with the listening. If this node is later deleted, the listening mechanism will stop.

To get a list of the events that can be listened to on an object, call function_s(object, "enumerateEvents", destNode). The object will dump a table onto
destNode that contains information on all the events that can be listened to on the object.

Some events may have "requirements". This means that in order to properly bind to the event, you have to pass additional parameters in. The enumeration
table will give information on the number of requirements and their names. When requirements are needed they displace and shift par1, par2, etc. For
example, if an event has 2 requirements, these requirements will take up par1 and par2 of the eventlisten() call, and par3 will then become param(1)
in relayNode's code.

## Example

```flexscript
eventlisten(current, "OnEntry", current.entrylistener);
```

