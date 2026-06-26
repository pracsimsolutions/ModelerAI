---
id: reference_propertiespanels_fixedresourcepanels_output_output
name: "Output"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","FixedResourcePanels","Output","Output"]
tags: ["manual","reference","propertiespanels","fixedresourcepanels"]
source: "manual/Reference/PropertiesPanels/FixedResourcePanels/Output/Output.html"
---

# Output

The Output panel defines how the object sends flow items downstream.

 

 

The following properties are on the Output panel:

 

### Send To Port

 

This picklist returns the output port number connected to the object that the flow item should be moved to. If 0 is returned, all outputs are opened and the flow item is moved to the first downstream object that is able to receive it.

 

### Use Transport

 

If this box is checked, the object will request a transport to move the flow item downstream. If it is not checked, the flow item will be moved automatically.

 

When you check the **Use Transport** box, an additional field is enabled where you define to whom the generated transport task sequence should be dispatched. The default will give the task sequence to the task executer/dispatcher connected to the object's first center port (current.centerObjects[1]), but you can custom-define this as needed.

 

### Priority

 

This parameter is only available if **Use Transport** is checked. This value sets the priority of the task sequence that will be sent to the transporter or dispatcher. Transporters and dispatchers generally sort task sequences so that sequences with higher priorities will be performed first. Task sequences with the same priority will be performed in the order that they were received.

 

### Preempt

 

This parameter is only available if **Use Transport** is checked. If set to one of the preempting values, the task sequences sent to the transporter will automatically preempt whatever the transporter is doing at the time. This may cause the transporter to perform tasks that would normally not be allowed, such as carrying more flow items than its capacity.
