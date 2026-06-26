---
id: reference_propertiespanels_taskexecuterpanels_taskexecuter_taskexecuter
name: "TaskExecuter"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","TaskExecuterPanels","TaskExecuter","TaskExecuter"]
tags: ["manual","reference","propertiespanels","taskexecuterpanels"]
source: "manual/Reference/PropertiesPanels/TaskExecuterPanels/TaskExecuter/TaskExecuter.html"
---

# TaskExecuter

The TaskExecuter panel controls how task executers handle flow items in the simulation model.

 

 

The following properties are on the TaskExecuter panel:

 

### Capacity

 

This number is the maximum number of flow items that the Task Executer can carry at a given time.

 

### Load Time

 

This property returns how long it takes this Task Executer to load the flow item.

 

### Unload Time

 

This property returns how long it takes this Task Executer to unload the flow item.

 

### Break to Requirement

 

This field is executed when the Task Executer comes to a break task or callsubtasks task. The return value is a reference to a task sequence. The logic within this field should search the Task Executer's task sequence queue, and find a task sequence that is appropriate to break to.

 

### Fire OnResourceAvailable at Simulation Start

 

If checked, the object's OnResourceAvailable trigger will be fired when the simulation starts.
