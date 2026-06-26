---
id: reference_propertiespanels_fixedresourcepanels_multiprocessor_multiprocessor
name: "MultiProcessor"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","MultiProcessor","MultiProcessor"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/MultiProcessor/MultiProcessor.html"
---

# MultiProcessor

The MultiProcessor panel controls the object's processing logic.

 

 

The following properties are on the MultiProcessor panel:

 

### Process List

 

The process list contains each of the processes for this multiprocessor. To edit a process, select it in the list and change its name and edit its properties below.

 

| Icon | Description |
| --- | --- |
|  | Click this button to add a new process to the process list. |
|  | Click this button to delete a process from the process list. |
|  | Click these buttons to move the selected process up or down in the process list. |

 

### Process Time

 

This picklist determines how long a processor spends processing a single flow item.

 

### Number of Operators

 

This number determines how many operators the object will use during its process time.

 

### Operator

 

This picklist returns a reference to the operator or dispatcher that the object is using during the given process.

 

### Priority

 

This value sets the priority of the task sequence that will be sent to the operator. Operators generally sort task sequences so that sequences with higher priorities will be performed first. Task sequences with the same priority will be performed in the order that they were received.

 

### Preemption

 

Sets the preempt value for calling operators.
