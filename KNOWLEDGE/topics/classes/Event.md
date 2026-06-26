---
id: event
name: "Event"
kind: class
module: ""
signature: "Event"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "An opaque awaitable type, specifically used in coroutines, representing an event owned by a specific object, that can be 'awaited', or listened to."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Event.xml"
---

# Event

An opaque awaitable type, specifically used in coroutines, representing an event owned by a specific object, that can be 'awaited', or listened to.

Event e = current.event(“OnEntry”);
await e;
await e; // awaiting the same Event object a second time will await the event a second time.
// The listening is initiated when the await statement executes, not when
// the event object is created. In other words, an instance of an Event object is
// a reference to an object’s event that may fire multiple times, not a single
// execution of an object’s event.

await current.event(“OnExit”);
await Event.evaluation(current.labels[“MyCode”]);
Event evaluationEvent = Event.evaluation(current.labels[“MyCode”]);
while (true) {
await evaluationEvent;
…
}

An event represents a generic reference to a specific object's OnEntry, OnExit, etc.,
event. It is not a
reference to a single execution of an object's event. This means you can await the
same Event multiple times, and each await may be resumed on different executions of the
object's event. In other words, the listening mechanism is initiated when the await statement
executes, not when the Event object is created.

## Methods

### evaluation

```flexscript
Event (static) Event.evaluation(treenode node)
```

Returns an Event object representing the evaluation of a node in the tree.

**Parameters:**
- `node` *(treenode)* — The node whose evaluation you want to listen for.

**Returns:** The resulting Event.

await Event.evaluation(current.labels["MyCode"]);

