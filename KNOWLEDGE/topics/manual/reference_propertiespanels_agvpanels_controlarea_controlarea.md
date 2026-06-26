---
id: reference_propertiespanels_agvpanels_controlarea_controlarea
name: "Control Area"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","AGVPanels","ControlArea","ControlArea"]
tags: ["manual","reference","propertiespanels","agvpanels"]
source: "manual/Reference/PropertiesPanels/AGVPanels/ControlArea/ControlArea.html"
---

# Control Area

The Control Area panel defines the position and type of the area.

 

 

The following properties are on the Control Area panel:

 

### Position, Size, Rotation

 

Specify the position, the X and Y size, and the Z rotation.

 

### Max Allocations

 

The maximum number of AGVs that can simultaneously claim the Control Area.

 

### Deallocation Type

 

The Control Area's assigned Deallocation Type. This defines the timing of when the object will be deallocated. Here you choose from a list of deallocation types that you can define in the AGV Network tool.

 

### AGV Member

 

Defines whether or not the control area is recognized as part of the AGV path network. When checked, AGV paths that cross the geometric boundaries of the control area will recognize those crossing points, and AGVs that travel those paths will consequently allocate and deallocate the control area.

 

Usually this box should be checked. Instances where you would uncheck it are if you want the control area to only be part of an A* network, or if you want to allocate the control area manually using [AGV.AllocatableObject.allocate()](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV.AllocatableObject&Method=allocate), and want it to be otherwise ignored in the AGV path network geometry.

 

### A* Member

 

Defines whether or not the control area is recognized as part of the A* network. When checked, A* grid nodes that are within the geometric boundaries of the control area will be flagged so that A* travelers who traverse those nodes will automatically allocate and deallocate the control area.

 

Note that A* travelers deallocate the control area as soon as their center point leaves the control area. In other words, they do not reference the control area's Deallocation Type. Also, a control area will be allocated based only on the A* grid nodes that it encompasses, so you should make sure the control area properly surrounds the correct set of grid nodes.
