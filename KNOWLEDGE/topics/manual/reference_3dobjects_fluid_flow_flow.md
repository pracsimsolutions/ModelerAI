---
id: reference_3dobjects_fluid_flow_flow
name: "The Flow Tab"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","Flow","Flow"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/Flow/Flow.html"
---

# The Flow Tab

## About the Flow Tab

 

The Flow Tab is only available on Fixed Resources. Use the Flow tab to control the pushing and pulling logic for 3D objects.

 

 

The following sections will explain the properties that are available on the Flow tab.

 

 

 

## Output

 

These properties determine how the object sends flow items downstream:

 

### Send To Port

 

This picklist returns the output port number connected to the object that the flow item should be moved to. If 0 is returned, all outputs are opened and the flow item is moved to the first downstream object that is able to receive it.

 

### Use Transport

 

If this box is checked, the object will request a transport to move the flow item downstream. If it is not checked, the flow item will be moved automatically.

 

When you check the **Use Transport** box, an additional field is enabled where you define to whom the generated transport task sequence should be dispatched. The default will give the task sequence to the task executer/dispatcher connected to the object's first center port (current.centerObjects[1]), but you can custom-define this as needed.

 

### Priority

 

This parameter is only available if **Use Transport** is checked. This value sets the priority of the task sequence that will be sent to the transporter or dispatcher. Transporters and dispatchers generally sort task sequences so that sequences with higher priorities will be performed first. Task sequences with the same priority will be performed in the order that they were received.

 

### Preempt

 

This parameter is only available if **Use Transport** is checked. If set to one of the preempting values, the task sequences sent to the transporter will automatically preempt whatever the transporter is doing at the time. This may cause the transporter to perform tasks that would normally not be allowed, such as carrying more flow items than its capacity.

 

### Reevaluate Sendto on Downstream Availability

 

If checked, the Send To Port will be re-evaluated every time a downstream object becomes available. It's important to note that this is only executed when the downstream object becomes available. It does not continuously evaluate just because a downstream object is already available. If you want to manually force a re-evaluation at some other time than when a downstream object becomes available, then you can do so by calling the openoutput() command on this object.

 

 

 

## Input

 

These properties define how an object pulls flow items from upstream objects.

 

### Pull

 

If this box is checked, the object will pull flow items from upstream objects. The upstream objects should open all their output ports to allow the object to pull the flow items it needs.

 

### Pull Strategy

 

This parameter is only visible if **Pull** is checked. This picklist returns the input port number connected to the object that the next flow item is to be pulled from. This field is evaluated only on reset of the model and when the pulling object becomes ready to receive its next flow item. For a Processor with a capacity of 1, this means that the Pull Strategy field will only be evaluated once right after each flow item exits the Processor.

 

### Pull Requirement

 

This parameter is only accessible if **Pull** is checked. This picklist needs to return either a true or a false (1 or 0). This field is evaluated when considering whether or not to pull in a particular flow item from the upstream object that was defined by the "Pull from port" field. This field will only be evaluated for flow items that are in the "ready" state (i.e.FRSTATE_READY) meaning the flow items are ready to leave the upstream object. Basically, the "Pull Requirement" field is evaluated for every "ready" flow item immediately after the "Pull from port" field gets evaluated. The field is evaluated again for each new flow item that later becomes ready in the upstream object.

 

### Reevaluate Pull Requirement on All Items When Each Upstream Item is Released

 

This parameter is only visible if **Pull** is checked. If checked, the object will re-evaluate the pull requirement for all released flow items upstream every time a new flow item is released. This is much like the Reevaluates Sendto on Downstream Availability check box, in that you may need to explicitly call openinput() if you want to manually trigger the re-evaluation of the pull requirement.
