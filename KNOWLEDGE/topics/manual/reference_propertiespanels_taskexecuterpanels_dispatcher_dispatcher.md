---
id: reference_propertiespanels_taskexecuterpanels_dispatcher_dispatcher
name: "Dispatcher"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","TaskExecuterPanels","Dispatcher","Dispatcher"]
tags: ["manual","reference","propertiespanels","taskexecuterpanels"]
source: "manual/Reference/PropertiesPanels/TaskExecuterPanels/Dispatcher/Dispatcher.html"
---

# Dispatcher

The Dispatcher panel controls how dispatchers handle task sequences.

 

 

The following properties are on the Dispatcher panel:

 

### Pass To

 

This property returns the output port number that the task sequence should be dispatched to. If 0 is returned, then the task sequence will be queued up using the below mentioned queue strategy, and then will be dispatched to the first available mobile resource. If -1 is returned, then the Dispatcher will do absolutely nothing. In such a case you would use the [TaskSequence.dispatch](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=dispatch)() and [TaskSequence.move](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=move)() methods to execute dispatching logic yourself.

 

### Queue Strategy

 

This property returns a "priority" value for the task sequence that is used to rank it in the object's task sequence queue. By default, it will simply return the priority value given to the task sequence when it was created, but the user can also customize task sequence priorities in this field.
