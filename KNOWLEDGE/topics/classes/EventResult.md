---
id: eventresult
name: "EventResult"
kind: class
module: ""
signature: "EventResult"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A type that holds information associated with a specific execution of an Event that has been awaited."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\EventResult.xml"
---

# EventResult

A type that holds information associated with a specific execution of an Event that has been awaited.

An EventResult allows you to get information out of a specific event execution. For example,
when listening to an object's OnEntry, you may want to know what item actually entered:

Event e = current.event(“OnEntry”);
var result = await current.event("OnEntry");
Object item = result.params["item"];
print("Item Entered: ", item.name);

## Properties

### params

```flexscript
EventResult.params
```

An array/map-like property that gives access to parameters of the event.

**Returns:** The resulting Event.

var result = await current.event("OnEntry");
int numParams = result.params.length;
Variant param1 = result.params[1];
Variant item = result.params["item"];

The name of a given parameter can be acquired by looking at an event's default code
(add the event to the object's triggers), and finding the names of the variables in the
code header. Also, for the parameter ranks,
you can refer to the various documentation of object events (for example,

FixedResource Events.

