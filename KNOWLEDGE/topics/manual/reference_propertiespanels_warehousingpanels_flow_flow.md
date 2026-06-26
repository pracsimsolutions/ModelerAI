---
id: reference_propertiespanels_warehousingpanels_flow_flow
name: "Flow"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","WarehousingPanels","Flow","Flow"]
tags: ["manual","reference","propertiespanels","warehousingpanels"]
source: "manual/Reference/PropertiesPanels/WarehousingPanels/Flow/Flow.html"
---

# Flow

The Flow panel defines allows the storage object to be used as a Fixed Resource.

 

 

The following properties are on the Flow panel:

 

### Use as Fixed Resource

 

If checked, the object will perform fixed resource logic, receiving items through its input ports, and releasing items to exit through its output ports. The Input and Output panels will display when checked.

 

### Max Content

 

This number defines how many flow items the rack will be allowed to hold at a given time.

 

### Minimum Dwell Time

 

This field returns a value of how long a flow item must stay in the rack before it is released to continue downstream. You can also return a value of -1 from this function so the rack will not release the item at all, and then implement your own releasing strategy using the releaseitem() command.
