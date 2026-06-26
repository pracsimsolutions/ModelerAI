---
id: connectingflows_conveyors_overviewconveyorobjects_overviewconveyorobjects
name: "Overview of Conveyor Objects"
kind: manual
breadcrumb: ["ConnectingFlows","Conveyors","OverviewConveyorObjects","OverviewConveyorObjects"]
tags: ["manual","connectingflows","conveyors","overviewconveyorobjects"]
source: "manual/ConnectingFlows/Conveyors/OverviewConveyorObjects/OverviewConveyorObjects.html"
---

# Overview of Conveyor Objects

## Library Objects

 

Conveyor objects are found in the Conveyors group in the FlexSim Library. The following table will provide a high-level overview of these objects. The categories and objects are presented in the same order as they appear in the Library.

 

| Name and Icon | Description | Appearance |
| --- | --- | --- |
| Straight Conveyor | The straight conveyor simulates the movement of items on a belt or accumulation conveyor. See Straight and Curved Conveyors for more information. |  |
| Curved Conveyor | The curved conveyor can join conveyors together, or you can use it to simulate a spiral elevating conveyor. See Straight and Curved Conveyors for more information. |  |
| Straight Mass Flow Conveyor | The mass flow corollary to a straight conveyor. See Mass Flow Conveyors for more information. |  |
| Curved Mass Flow Conveyor | The mass flow corollary to a curved conveyor. See Mass Flow Conveyors for more information. |  |
| Join Conveyors | Join conveyors acts more like a tool than an object. Use it to create a curved conveyor connecting two conveyor sections. See Join Conveyors for more information. |  |
| Photo Eye | Photo eyes are objects you place at various points in your conveyor system. They simulate light-beam-based photo sensors in that they trigger logic when their 'beam' is covered by flow items. You can define block times and clear times for detecting accumulation, or have the photo eyes fire logic on each flow item that blocks them. You can also make precise adjustments to the height and angle of the photo eye on the conveyor, to make it simulate an angle or elevated photo eye. See Photo Eye for more information. |  |
| Decision Points | Decision points are similar to photo eyes, but are triggered by each passing item, instead of being based on the simulated blocking of a light beam. While there is significant overlap as to when you use decision points versus photo eyes, in general you use a decision point when you need logic to fire on every passing flow item, and you use a photo eye when you want to simulate the covering/uncovering of a photo sensor. See Decision Point for more information. |  |
| Stations | Stations are similar to decision points and photo eyes in that you place them at certain points in your conveyor system. Stations are specifically for stopping and 'processing' an item on a conveyor. Processing may involve a simple imposed delay time, or you may require an operator for processing. In this way stations are similar to the library's Processor object. See Station for more information. |  |
| Motor | Motors can be used to control whether the conveyor systems are on or off at a given time. The motor can also be used to sync dog gaps on a power and free chain loop when simulating a power and free conveyor system. See Motor for more information. |  |
| Merge Controller | The merge controller is an object that can control how different conveyor lanes merge together. Use the merge controller to define a lane release strategy for the merge. See Merge Controller for more information. |  |

 

 

 

## Additional Conveyor Objects

 

In addition to these objects which are available in the FlexSim Library, there are three additional conveyor objects you should be aware of: transfers, entry transfers, and exit transfers. These objects are not available in the FlexSim library but are automatically created whenever you connect conveyors to other objects. See Working With Transfer Objects for more information.
