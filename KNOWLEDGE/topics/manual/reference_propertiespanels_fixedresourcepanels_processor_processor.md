---
id: reference_propertiespanels_fixedresourcepanels_processor_processor
name: "Processor"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","Processor","Processor"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/Processor/Processor.html"
---

# Processor

The Processor panel controls the object's setup and processing logic.

 

 

The following properties are on the Processor panel:

 

### Max Content

 

This number defines the number of flow items that the processor can hold at one time.

 

### Animate Items

 

If this box is checked, flow items will be seen translating from one side of the processor to the other as their process time elapses. It is for visualization purposes only. If unchecked, entering flow items will be placed in the middle of the processor and remain until exiting.

 

### Setup Time

 

This picklist defines the amount of time that the object waits after receiving a flow item to begin processing that flow item.

 

### Use Operator(s)

 

If this box is checked the object will call for one or more operators during its setup time. The operator(s) will be released after the setup time has expired. The number determines how many operators the object will use during its setup time.

 

### Process Time

 

This property determines how long a processor spends processing a single flow item.

 

### Use Operator(s)

 

If this box is checked the object will call for one or more operators during its processing time. The operator(s) will be released after the process time has expired. The number determines how many operators the object will use during its process time.

 

### Same as Setup

 

This property is only visible if both the **Use Operator(s)** boxes are checked. If this box is checked, the operators that were called for setup time will be utilized during process time. If this box is not checked, the operators used for the setup time will be released and new operators will be called for the process time. Different operators can be called using a special pick option in the **Operator** parameter.

 

### Operator

 

This property returns a reference to the operator or dispatcher that the object is using during setup or process time.

 

### Priority

 

This value sets the priority of the task sequence that will be sent to the operator. Operators generally sort task sequences so that sequences with higher priorities will be performed first. Task sequences with the same priority will be performed in the order that they were received.

 

### Preemption

 

Sets the preempt value for calling operators. This may cause the operator to perform tasks that would normally not be allowed.
