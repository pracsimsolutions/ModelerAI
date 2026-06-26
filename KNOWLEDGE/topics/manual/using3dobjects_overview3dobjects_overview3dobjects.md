---
id: using3dobjects_overview3dobjects_overview3dobjects
name: "Overview of 3D Library Objects"
kind: manual
breadcrumb: ["Using3DObjects","Overview3DObjects","Overview3DObjects"]
tags: ["manual","using3dobjects","overview3dobjects","overview3dobjects"]
source: "manual/Using3DObjects/Overview3DObjects/Overview3DObjects.html"
---

# Overview of 3D Library Objects

## Introduction to 3D Library Objects

 

The 3D objects in the FlexSim Library are the basic building blocks you will use to build your 3D model. Each object has built-in logic that is commonly used in a variety of simulation models. You can also easily edit the properties and customize the logic on these objects to adapt any of them to the unique needs of your simulation project.

 

This topic will provide a high-level overview of the objects in the FlexSim Library. Each object has a link to its object reference page for more information. The categories and objects are presented in the same order as they appear in the Library.

 

 

 

## Fixed Resources

 

Fixed resources are objects that remain fixed or stationary in your model. Generally speaking, fixed resources interact with flow items in the simulation, such as storing or modifying flow items. Fixed resources can represent various steps or processes in your model, such as processing stations, machines, or storage areas.

 

| Name and Icon | Description |
| --- | --- |
| Source | The source creates flow items and releases them to a downstream object. You can control the rate at which the source creates flow items so that they arrive on a fixed schedule, a regular continuous rate, or a random statistical distribution. See the Source reference page for more information. |
| Queue | The queue stores flow items until a downstream object is ready to take them. By default, the queue releases flow items on a first-in-first-out basis, but other options are available. See the Queue reference page for more information. |
| Processor | Processors simulate flow items getting processed at a station. Processors simulate a time delay, beginning with a setup time followed by the process time. You can also require the processor to use an operator during the setup and/or process time. You can also set processors to handle more than one flow item at a time. See the Processor reference page for more information. |
| Sink | The sink removes flow items from a simulation model when they are finished. See the Sink reference page for more information. |
| Combiner | The combiner groups multiple flow items together. It can either join the flow items together permanently, or it can pack them into a container flow item so that they can be separated at a later point in time. See the Combiner reference page for more information. |
| Separator | The separator separates a flow item into multiple parts, either by unpacking a container flow item that has been packed by a combiner or by making multiple copies of the original flow item. The splitting/unpacking is done after the process time has completed. See the Separator reference page for more information. |
| Multiprocessor | The multiprocessor is similar to the processor object, but it can simulate flow items going through a sequence of two or more processes. You can require the multiprocessor to use an operator in any or all of these processes. See the MultiProcessor reference page for more information. |
| BasicFR | You can use the BasicFR object to create a custom fixed resource object. You can find it in the Advanced section of the Library. See the BasicFR reference page for more information. |

 

 

 

## Task Executers

 

Task executers are objects that can move throughout the model and interact with fixed resources and flow items. They are called task executers because they can be assigned tasks and task sequences. For example, these objects can travel, load flow items, unload flow items, act as shared resources for processing stations, and perform many other simulation tasks.

 

All task executers have the same basic functionality; the main difference between them is the way they move.

 

| Name and Icon | Description |
| --- | --- |
| Dispatcher | The dispatcher is used to control a group of transporters or operators. Fixed resources can send task sequences to the dispatcher, which then delegates the tasks to the transports or operators that are connected to it once they become available. See the Dispatcher reference page for more information. |
| TaskExecuter | The TaskExecuter object is a generic task executer. A TaskExecuter has some basic functionality that you can adapt to a variety of uses. This object's default shape is designed to look like an automatic guided vehicle (AGV). See the Task Executer (Object) reference page for more information. |
| Operator | Operators represent employees that can transport flow items, operate fixed resources, and perform a variety of other tasks that require an employee in a simulation model. See the Operator reference page for more information. |
| Transporter | The transporter is used mainly to carry flow items from one object to another. It has a fork lift that will raise or lower flow items when it is loading or unloading them from a rack. See the Transporter reference page for more information. |
| Elevator | The elevator is a special type of transport that moves flow items up and down. It will automatically travel to the level where flow items need to be picked up or dropped off. Flow items are animated as they enter and exit the elevator to improve model accuracy. See the Elevator reference page for more information. |
| Robot | The robot is a special transport that lifts flow items from their starting locations and places them at their ending locations. Generally, the robot's base does not move. Instead, 6 joints rotate to move the flow items. See the Robot reference page for more information. |
| Crane | The crane simulates rail-guided cranes such as gantry, overhead, or jib cranes. It has similar functionality to the transporter but with modified graphics. By default, the crane picker rises to the height of the crane object after picking up or dropping off a flow item before it will travel to the next location, but these motions can be customized. See the Crane reference page for more information. |
| ASRSvehicle | The ASRS vehicle is a special type of transport specifically designed to work with racks. The ASRS vehicle will slide back and forth in an aisle between two racks picking up and dropping off flow items. The reach, lift, and travel motions are fully animated by the ASRS vehicle. The lift and travel motions will occur simultaneously, but the reach will only occur after the vehicle has come to a complete stop. See the ASRSvehicle reference page for more information. |
| BasicTE | You can use the BasicFR object to create a custom task executer object. You can find it in the Advanced section of the Library. See the BasicTE reference page for more information. |

 

 

 

## Travel Networks

 

These objects will be explained in the chapter about task executers and travel. See Working With Travel Networks for more information.

 

 

 

## Conveyors

 

These objects will be explained in the chapter about conveyors. See Overview of Conveyor Objects for more information.

 

 

 

## Warehousing

 

Warehousing objects allow you to simulate the storage and retrieval of items in a warehouse or storage system. See Rack Types for more information.

 

 

 

## Visual

 

The visual objects that are available in the FlexSim library can help you manage complex models and add visual details that might be useful when presenting the model to stakeholders. See Using Visual Objects for more information.

 

 

 

## A* Navigation

 

These objects will be explained in the chapter about task executers and travel. See Working With A* Navigation for more information.

 

 

 

## AGV

 

These objects will be explained in the chapter about task executers and travel. See Key Concepts About AGV Networks for more information.

 

 

 

## People

 

These objects will be explained in the chapter about healthcare objects. See Using Healthcare 3D Objects for more information.

 

 

 

## Fluid

 

The fluid objects are designed to simulate fluids or other materials that behave like fluids that are measured by weight or volume.

 

| Name and Icon | Description |
| --- | --- |
| Fluid Ticker | The ticker is responsible for breaking time into small, evenly spaced units called ticks. The ticker can control all of the fluid objects in a model. See the Fluid Ticker reference page for more information. |
| Fluid Tank | The fluid tank is a simple fluid object that can receive and send material at the same time. You can configure up to three points (called *marks*) that will cause triggers to fire when the tank's capacity reaches those specific volumes. See the Fluid Tank reference page for more information. |
| Fluid Generator | The fluid generator provides fluid material for a model, similar to a source. The generator can be set to refill at a fixed rate or it can refill itself a set amount of time after it becomes empty. See the Fluid Generator reference page for more information. |
| Fluid Terminator | The fluid terminator removes fluid material from a simulation model when these materials are finished. See the Fluid Terminator reference page for more information. |
| Fluid Mixer | The fluid mixer combines products together into a single, new product. The different materials can either be pulled sequentially or in parallel. The mixer always works in batches. It does not send any material until it has received and processed all the material that it was set to receive. See the Fluid Mixer reference page for more information. |
| Fluid Blender | The fluid blender mixes materials from multiple input ports based on percentages (not fixed amounts). It is most commonly used for in-line blending where the mixing is not done in batches. See the Fluid Blender reference page for more information. |
| Fluid Splitter | The fluid splitter sends material to multiple output ports in percentages that the modeller specifies. See the Fluid Splitter reference page for more information. |
| Fluid Pipe | The fluid pipe simulates the time required to move material from one object to another. It can appear as either a cylindrical pipe, or as a simple conveyor. See the Fluid Pipe reference page for more information. |
| Fluid Processor | The fluid processor simulates a processing step that continuously receives and sends fluid material (such as a continuous cooker). See the Fluid Processor reference page for more information. |
| Item to Fluid | The Item to Fluid object receives flow items and converts them to fluid material. See the Item to Fluid reference page for more information. |
| Fluid to Item | The Fluid to Item receives fluid and converts it to flow items that it sends downstream to a fixed resource. See the Fluid to Item reference page for more information. |
| Fluid Conveyor | The Fluid Conveyor controls fluid flow through the use of multiple input and output ports. Inputs and outputs may be placed anywhere along the length of the conveyor. The direction, speed, acceleration and angle of repose along with the length and width of the conveyor are all factors that will affect where material rests inside the conveyor and when and to which output port the material will be sent. See the Fluid Conveyor reference page for more information. |
