---
id: stopobject
name: "stopobject"
kind: command
module: ""
signature: "stopobject(obj object, num state [, num id, num priority, num stateprofile] )"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.stop"
description: "Deprecated, use Object.stop()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stopobject

> ⚠ **DEPRECATED.** Use `Object.stop` instead.

```flexscript
stopobject(obj object, num state [, num id, num priority, num stateprofile] )
```

Deprecated, use Object.stop()

This command is deprecated. Use Object.stop() instead.

Tells the object to stop whatever it is doing, go into the state specified (in the state profile if specified), and waits for resumeobject() to be called. Stopobject commands are accumulated, meaning if stopobject is called twice
on the same object, the object will not resume its operations until resumeobject has been called twice as well. Stopping an object depends on the type of object that is being stopped.
For FixedResources,
generally events are delayed indefinitely, input and output is stopped, and all operations going into and out of the object are stopped. This means that TaskExecuters trying to load/unload to or from the object
will have to wait until the object has been resumed.
For TaskExecuters, events are not delayed, but rather a preempting task sequence with priority of 100,000 is created for the TaskExecuter, with one
TE_STOP task in it.
Be aware that, if there are several stop requests for the same object, the state for each stop request is not remembered. If an object is requested to stop by entity A with state 12,
and then is later requested to stop by entity B for state 14, it will go into state 14 and forget state 12. Even if entity B resumes the object before entity A, the object will remain in state 14 until all stop
requests have resumed.
The last two parameters, id and priority, are optional and have recently been added to fix the problem in the previous paragraph. If you specify these parameters, then it will store
a record of the stop request, instead of just incrementing a value for the number of stops requested. The id is like a key for that stop request. The priority allows the object to prioritize its stop requests.
For example, if you have a stop request for the scheduled down state, and another for the breakdown state at the same time, technically the object should be in two states at once, but since a FlexSim object can
only be in one state at a time, the priority value breaks the tie and goes into the state of the highest priority stop request.
The id value should match an id value that is added to a resumeobject() command
later on. This is for matching stop requests with their appropriate resume requests. For example, if you stop an object for scheduled maintenance with and id of 1, once the scheduled maintenance is finished,
you will need to resume the object with the same id of 1.

## Example

```flexscript
stopobject(current.centerObjects[1], STATE_BLOCKED);
```

