---
id: connectingflows_overview3dflows_overview3dflows
name: "Overview of 3D Object Flows"
kind: manual
breadcrumb: ["ConnectingFlows","Overview3DFlows","Overview3DFlows"]
tags: ["manual","connectingflows","overview3dflows","overview3dflows"]
source: "manual/ConnectingFlows/Overview3DFlows/Overview3DFlows.html"
---

# Overview of 3D Object Flows

## What Are Flows?

 

In the previous chapter about 3D objects, you learned about the three basic types of 3D objects:

 

1. **Flow items** - The items or materials that will flow through your simulation model. Flow items can represent materials, products, customers, orders, information, etc.
1. **Fixed resources** - Objects that remain fixed or stationary in your model as they interact with flow items. Fixed resources can represent workstations, storage centers, processing stations, etc.
1. **Task executers** - Objects that move around the model performing various tasks such as transporting flow items, operating machines, etc.

 

In this chapter, you'll learn more about how these objects interact together in a simulation model to create a *flow*. Generally speaking, flows are the movement of items throughout the simulation from the beginning to end:

 

 

Some practitioners might also refer to this process as *item routing*, which is the logic that determines how items move from one station to another in a business system.

 

If you're familiar with Lean philosophy, you'll know that business systems can be improved by studying and optimizing the flow of items (or customers) throughout the business system. Business systems can be said to be more "lean" when they create smooth and continuous flows wherever possible. Lean systems eliminate waste created by overproduction, bottlenecks, and idleness. Therefore, learning different ways of creating flows between 3D objects in FlexSim will enable you to study and analyze different potential real life flow schemes, improving your business system's efficiency.

 

 

 

## Key Terms

 

Before learning about flows, you should be aware of a few key terms and concepts that are frequently used when discussing flows:

 

### Upstream and Downstream

 

From a certain perspective, you could think of flows as a *stream* of items. In the same way that water flows down a river or a brook, items can flow through a business system as they are transferred from one 3D object to another. For that reason, you might occasionally see the terms *upstream* and *downstream* to refer to different objects that are either further up or down the flow.

 

For example, in the following image, three sources create flow items and transfer those items to conveyors. Then the combiner receives the flow items and packs them into a tote. In that sense, the three sources could be said to be *upstream*, while the combiner could be said to be *downstream*:

 

 

### Pushing and Pulling

 

The terms *pushing* and *pulling* refers to the exchange of flow items between two objects. It describes whether the downstream object or the upstream object controls the flow of items. An upstream object *pushes* flow items when it is sending the flow item to the next available downstream object, which means the upstream object controls the flow. An downstream object *pulls* flow items when it is taking a flow item from an upstream object, which means the downstream object controls the flow.

 

For example, the following simulation model has objects that both push and pull:

 

 

In this example, the source creates three different types of flow items and *pushes* them to the first queue. The source can immediately push them to the queue because the queue can accept flow items at any point in time.

 

The three processors then *pull* the items from the queue as soon as two conditions are met:

 

1. The processor is empty and ready to process another item
1. A flow item that meets its criteria is available in the queue

 

In this case, each processor pulls items based on their item type. (Processor1 pulls type 1 items, Processor2 pulls type 2 items, etc.) Ultimately, the processors control the flow of items from the queue to the processors, which is why the processors could be said to *pull* the items from the upstream queue.

 

You can build logic into your simulation model to determine whether an object should push or pull flow items. By default, most objects are set to push flow items rather than pull them, but these settings can be changed.

 

 

 

## Common Types of Flows

 

FlexSim has the ability to build nearly every type of flow connection or any kind of flow logic. The following sections explain some of the more common types of flow connections used in simulation modeling.

  

#### One-to-Many or Many-to-Many Connections

 

With the exception of the one-to-one connection type described in the first section, most of the methods explained in the following sections apply to one-to-many or many-to-many connections.

  

### One-to-One

 

The most basic type of flow connection is a single connection between one object and another. In this type of connection, an upstream object will either push a flow item to the next downstream object as soon as it is available to accept it or the downstream object will pull the flow item when it is ready for it:

 

 

### First Available

 

When objects use the first available logic, flow items will move to the first downstream object that is available to receive the flow item.

 

For example, if the first port (port 1) is available, the flow item will be sent there. If the first port is unavailable, the flow item will be sent to the second port, and so forth. If no ports are available, the flow item will wait until a port is available:

 

 

### Round Robin

 

When objects use the round robin logic, flow items will be sent to each available downstream objects in turn. The first flow item will move to the first object, the second flow item will move to the second object, and so on:

 

 

### Random

 

You could assign flow items to flow to downstream objects at random. You could possibly use a random statistical distribution to divert flow items to one object or another. FlexSim is capable of generating randomness using different distributions such as a coin flip (bernouilli), any number within a range of values, a certain percentage of objects to one object versus another, etc.

 

In the following example, the first queue is set to randomly push 70% of flowitems to the second processor, 15% to the first processor, and 15% to the third processor:

 

 

### Conditional

 

Objects can use conditional logic to determine which items should flow to certain downstream objects. For example, objects could be sent to a downstream object based on certain priorities, such as how long they've been waiting in a queue or by their item type. Objects with a specific value on a custom label could be sent to a specific downstream object (such as based on their weight, their size, or some other value on a label). The way that flow items get routed could also possibly depend on other external conditions in the simulation model such as the time of day or specific circumstances. Nearly any kind of condition can be simulated, so the possibilities are limitless.

 

In the following example, each processor conditionally pulls items based on their item type. Processor1 pulls type 1 items, Processor2 pulls type 2 items, etc.:

 

 

### Sequential

 

Objects can use global tables and lists to send flow items to objects in a specific order or sequence.

 

For example, imagine you wanted to simulate a job shop in which items will be sent to one of four different stations (drilling, assembly, welding, pressing). Depending on the type of item it is, it will need to be sent to each station in a different order:

 

 

You can use a combination of a global table and a list to create these kinds of complex sequential flows:

 

 

 

 

## Using Transports

 

Objects typically flow from one fixed resource to another in a 3D model. Unlike fixed resources, task executers such as operators, robots, AGVs, etc. can move throughout the simulation model. For that reason, you can use task executers to transport flow items from one fixed resource to another, as shown in the following image:

 

 

 

 

## Overview of Flow Connection Tools

 

FlexSim has a variety of tools you can use to create flows between 3D objects, as explained in the following sections:

 

### Ports

 

You can create port connections directly between 3D objects in the simulation model. You can then use the properties on the 3D objects to control the flow between two objects using their ports.

 

 

Port connections work well when you're just sending an item from one object to a single downstream object (such as a one-to-one connection type). Port connections between two objects can be created quickly and easily and come pre-programmed with basic flow logic.

 

Port connections can also be ideal if you're sending an item from one object to one of many upstream or downstream objects using a basic routing method such as first available, round-robin, random and some kinds of conditional routing.

 

See Key Concepts About Ports for more information.

 

### Lists

 

An upstream object can push flow items onto a list and a downstream object can pull those items from the list. The downstream object can filter or prioritize certain list items over others.

 

In the following example, the first queue pushes its flow items onto a list called Available Products. The items wait in the queue until they are pulled from the list by the processors. The processors pull items that match their item type query. For example, Processor1 will only pull type 1 items. Processor2 will only pull type 2 items, etc.

 

 

Lists work well when it might become too unwieldy or difficult to use port connections, such as when you need to have many-to-many routing. Lists can also be ideal when you need to filter or prioritize flow items using a fairly complex set of rules. In these cases, using lists to create the criteria for sending and receiving items can be easier than using ports.

 

See Key Concepts About Lists for more information.

 

### Conveyors

 

You can use the conveyor objects to create a variety of conveyor systems including slug-building systems, power and free systems, and conditional routing systems. For example, the following conveyor system creates slugs of six objects before releasing them to the downstream objects:

 

 

See Key Concepts About Conveyors for more information.

  

#### Conveyor Systems Use Ports

 

Conveyor objects need ports to connect to non-conveyor objects and to connect conveyor logic objects. Therefore, it's important to learn about ports if you want to use conveyors in your simulation model.

  

### Process Flow

 

Process Flow is a tool that is primarily used to control the logic of the simulation model, but you can use it to move flow items between objects as well, if needed.

 

The following example shows how you would create a basic flow between objects in process flow:

 

 

The Process Flow tool is very versatile. As such, using the Process Flow tool is ideal if you need to create a custom flow between objects that difficult to create using one of the other methods. See Overview of the Process Flow Interface for more information.

 

### AGVs

 

Automatic guided vehicles (AGVs) are portable robots designed to transport goods from one destination to another. AGV systems are now being used for a growing variety of applications in materials handling, manufacturing, and beyond. You could use the AGV objects and tools to if you would like to experiment with AGV transports. See Key Concepts About AGV Networks for more information.
