---
id: workingwithtasks_agvnetworks_keyconceptsagvnetworks_keyconceptsagvnetworks
name: "Key Concepts About AGV Networks"
kind: manual
breadcrumb: ["WorkingWithTasks","AGVNetworks","KeyConceptsAGVNetworks","KeyConceptsAGVNetworks"]
tags: ["manual","workingwithtasks","agvnetworks","keyconceptsagvnetworks"]
source: "manual/WorkingWithTasks/AGVNetworks/KeyConceptsAGVNetworks/KeyConceptsAGVNetworks.html"
---

# Key Concepts About AGV Networks

## Reasons to Simulate AGV Systems

 

FlexSim's AGV objects and tools are designed to help you simulate business systems that will use automated guided vehicles (AGVs). AGVs are portable robots designed to transport goods from one destination to another.

 

The following image shows a hypothetical example of a hospital that uses AGVs to transport medical supplies, laundry, and waste:

 

 

AGV systems are now being used for a growing variety of applications in material handling, manufacturing, and beyond. Since AGV systems are costly to implement, they are ideal for a simulation project. You can experiment with different AGV system setups in a simulation model until you find the most efficient system for your needs.

 

An AGV simulation model can also act as a communication tool when you get to the implementation phase. You can hand off the model to the individuals who will program the AGVs to help them understand the kind of logic that they will need to use when they program the AGVs in the system.

 

 

 

## Before Building an AGV Network

 

Before beginning your AGV simulation project, you should ideally research the types of AGVs your system will use. If it's not possible to find out information about the AGV system before you begin your project, keep in mind that you can build a working model and then make modifications to it as you get more information about the AGVs you will use.

 

With that in mind, the following is a list of questions that you could consider asking your vendor about the AGVs that you'll use in your system:

 

- **Navigation** - What kind of navigation system will your AGVs use? How will vehicles enter and exit the system? Will operators need to call AGVs or will they loop on a continuous system looking for work?
- **Routing** - How will your AGVs be routed from one location to another? Will your system use a central AGV controller? How will AGV traffic be controlled to prevent collisions?
- **Vehicle Specifications** - What is the load capacity of the AGVs? How long does it take to load and unload? At what speed do the AGVs travel, including when decelerating or turning? Do their speeds change when they are carrying a load? What is their battery lifespan and charge rate?

 

 

 

## Overview of AGV Objects and Tools

 

The following sections will provide a high-level overview of the 3D objects and tools that are used to build an AGV model in FlexSim.

 

### Library Objects

 

The TaskExecuter 3D object in the library is designed to look and move like an AGV, so it is the best object to use in an AGV model. You can find this object in the Task Executer category of the library.

 

 

The AGV category in the library has the following objects:

 

| Name and Icon | Description |
| --- | --- |
| Straight Path | Draws a straight path that AGVs will use to travel from one location to another. |
| Curved Path | Draws a curved path that AGVs will use to travel from one location to another. |
| Join Paths | Join paths acts more like a tool than an object. Use it to create a curved AGV path connecting two AGV paths. |
| Control Points | Control points handle a lot of the logic in AGV networks. AGVs and flow items will need to enter and exit the network through a control point. If you need to route AGVS to different floors, you'll use control points to communicate with elevator objects. Control points can also be connected to other control points to create look for work systems where AGVs loop through the network looking for tasks at different locations. |
| Control Area | You can add control areas to sections of the AGV network to prevent collisions. These control areas will restrict access to that section of the AGV network by only allowing a fixed number of AGVs into the area at a time. |

 

### Process Flow Templates

 

To make AGV simulation even simpler, FlexSim has several pre-built AGV process flow templates you can use with your AGV simulation project. These process flow templates already have basic logic to simulate many common AGV systems, but also can act as a launching point for you to customize the logic to your specific AGV system's needs. All you need to do is choose which process flow is closest to how your system works, then attach the AGVs in your 3D model to the chosen process flow, then adjust the settings or add new logic that will be unique to your AGV system.

 

See Using the AGV Process Flow Templates for more information on process flows for controlling and dispatching AGVs.

 

See Adding Elevators to AGV Networks for more information on a process flow that integrates elevators into AGV travel.

 

 

 

## How AGVs Work in FlexSim

 

AGVs are essentially task executers that have been connected to the AGV path network. As such, all travel tasks will be executed via the AGV travel network. For that reason, you should consider reading Key Concepts About Task Logic before reading about AGVs specifically.

 

Other than defining the specific logic of an AGV's travel tasks, generally all other tasks are handled according to normal task executer logic. This is no different than if you were to connect the task executer to an A* travel network. However, when you connect the AGV to an AGV Process Flow Template this enables special dispatching and task sequence generation logic that is specifically catered to AGV systems.

 In other words, merely connecting the task executer to the AGV network only affects the Transportation or Travel aspect of a task executer's behavior. However, when you connect the AGV to an AGV Process Flow Template, you are defining specific solutions associated with the Task Sequence Generation and Task Sequence Dispatching aspects of the task executer's behavior.
