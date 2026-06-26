---
id: using3dobjects_typesofobjects_typesofobjects
name: "Types of 3D Objects"
kind: manual
breadcrumb: ["Using3DObjects","TypesOfObjects","TypesOfObjects"]
tags: ["manual","using3dobjects","typesofobjects","typesofobjects"]
source: "manual/Using3DObjects/TypesOfObjects/TypesOfObjects.html"
---

# Types of 3D Objects

## Introduction to Types of 3D Objects

 

FlexSim offers a variety of different 3D objects that you can use to build a simulation model. The three most important categories of objects you need to know about are:

 

1. Flow items
1. Fixed resources
1. Task executers

 

There are more types of objects besides these three, but these categories are the most important for you to understand as a beginner. This topic will explain what these types of objects are and how they interact together. It will also include a brief overview of the other types of objects at the end.

 

 

 

## Flow Items

 

Flow items are the items that will flow through your simulation model from the beginning of your simulation until the end. Flow items can represent many different things in your simulation model such as:

 

- Materials moving through an assembly line
- Customers walking through a service center
- Products moving to different locations in a warehouse
- Orders getting transferred from one station to another

 

By default, flow items look like brown boxes, as shown in the following image:

 

 

However, you can easily change the visual appearance of flow items if needed. See Key Concepts About Flow Items for more information about flow items, such as how to add flow items to a 3D model.

 

 

 

## Fixed Resources

 

Fixed resources are objects that remain fixed or stationary in your model, as shown in the following image:

 

 

Generally speaking, fixed resources interact with flow items in the simulation, such as storing or modifying flow items. Flow items will flow from one fixed resource to the next downstream fixed resource until they reach the end of the simulation:

 

 

Fixed resources can represent various steps or processes in your model, such as:

 

- Work stations
- Machine operations
- Processing stations
- Loading and unloading sites
- Storage areas

 

Several different fixed resources are available in the FlexSim library. (See Overview of 3D Library Objects - Fixed Resources for more information.) You can also use these objects as a jumping off point to build your own custom fixed resources. (See Creating and Importing Custom 3D Objects for more information.)

 

 

 

## Task Executers

 

Task executers are objects that can move throughout the model and interact with fixed resources and flow items. All task executers have the same basic functionality; the main difference between them is the way they move.

 

 

They are called task executers because they can be assigned tasks and task sequences. For example, these objects can travel, load flow items, unload flow items, set up or operate machines, act as shared resources for processing stations, and perform many other simulation tasks:

 

 

Task operators can represent workers, AGVs, vehicles or machinery that transport items, etc.

 

Like fixed resources, the FlexSim library has a variety of task executers to choose from. (See Overview of 3D Library Objects - Task Executers for more information.) You can also build your own custom task executers. (See Creating and Importing Custom 3D Objects for more information.)

 

 

 

## Other Types of 3D Objects

 

Beyond these three basic categories of items in FlexSim, there are additional types of objects that you can use to improve your simulation model:

 

- **Visual** objects can help you manage complex models and add visual details that might be useful when presenting the model to stakeholders.
- **Conveyor** objects can be used to create conveyor systems to transport flow items through the simulation model.
- **AStar** and **AGV** objects control task executer movement in the simulation model, making sure that task executers use designated travel paths.
- **Fluid** objects can be used for creating simulation models for handling fluid materials.
