---
id: connectingflows_ports_keyconceptsports_keyconceptsports
name: "Key Concepts About Ports"
kind: manual
breadcrumb: ["ConnectingFlows","Ports","KeyConceptsPorts","KeyConceptsPorts"]
tags: ["manual","connectingflows","ports","keyconceptsports"]
source: "manual/ConnectingFlows/Ports/KeyConceptsPorts/KeyConceptsPorts.html"
---

# Key Concepts About Ports

## Introduction to Ports

 

When you hear the term *port*, you might typically think of a docking station where freighter ships load and unload a shipment of goods. That's actually very similar to how ports function in FlexSim.

 

In FlexSim, a port is a point on an object where flow items can be transferred from one object to another. A port connection is the connection between ports on two different objects. Ports create relationships between objects that allow them to communicate with each other and exchange flow items.

 

 

The biggest advantage of using port connections is that they are quick and easy to create. Ports also have fairly straight-forward flow logic, which makes it easy for new users to understand how they work.

 

However, if you want to create complicated, many-to-many flows between objects or if you want to create your own custom flow logic, it might be better to use lists or Process Flow. That being said, it's still beneficial to have a good understanding of how ports work because they form a good foundation for understanding flow logic in general.

 

The rest of the sections in this topic will discuss the key concepts you will need in order to understand how ports function.

 

 

 

## Types of Port Connections

 

You will need to use different types of port connections depending on the kinds of objects you're connecting, such as whether you're connecting fixed resources or task executers. (See Types of 3D Objects for more information about these terms.)

 

The two most common types of port connections are input/output and center port connections. The following sections explain the difference between these types of port connections.

  

#### Alternate Terms for Port Connections

 

Occasionally, FlexSim users might refer to an input/output port connection as an *A-connect* and a center port connection as an *S-connect*. These terms are sometimes used because the A key is used as a shortcut to create input/output port connections and the S key is used as a shortcut for creating center port connections. See Adding and Removing Ports for more information.

  

### Input/Output Ports (A-Connects)

 

Input/output ports are the most common types of port connections. These ports are usually used to connect two fixed resources together so that they can exchange flow items. The output port of an upstream object is connected to the input port of a downstream object. An output port is where the flow item exits the object and an input port is where the flow item enters the object.

 

Input/output ports are graphically represented as a small red or green triangle. Input ports will look like an arrow pointing in toward the object. Output ports will look like an arrow pointing away from the object.

 

 

### Center Ports (S-Connects)

 

Center ports are usually used to connect task executers to fixed resources, but they can connect any two objects that need to reference each other. When the center ports of two objects are connected, it creates an abstract reference point between those two objects. Center ports enable objects to communicate or interact in complex ways:

 

- **Transporting flow items** - Fixed resources can use the task executers connected to their center ports to transport flow items to a downstream fixed resource.
- **Setting up and processing** - Some fixed resources have setup and processing times (processors, combiners, separators, multi-processors). These objects can require the presence of a task executer connected to their center ports during setup and processing times.
- **General reference** - Objects can have center port connections in order to communicate with or reference each other.

 

In FlexSim, center ports are graphically represented as a red square:

 

  

#### Extended Connections

 

FlexSim has another type of connection known as an extended connection. Only network node and traffic control objects can use extended connections. You might possibly use extended connections to build travel networks, but you will probably only use them in rare circumstances. For that reason, this chapter will focus primarily on input/output and center port connections.

  

 

 

## Port Rankings

 

You need to understand how ports are ranked in FlexSim if you want to use ports to create certain types of flows such as round robin, random, conditional, etc. See Common Types of Flows for more information.

 

Every time you create a port connection between two objects, FlexSim automatically assigns a rank to that connection. The first port connection you make will have a rank of 1, the second will have a rank of 2, etc.

 

For example, the queue in the following image has three output ports. The output port going to Processor1 is ranked 1, the output port to Processor2 is ranked 2, and the output port to Processor3 is ranked 3. Notice that the output ports are visually arranged on the queue according to their ranking order:

 

 

You could also view the port rankings by clicking on the queue to bring up Properties. In the Ports panel, select Output Ports to view the rankings:

 

 

See Changing Port Rankings for more information.

 

 

 

## Open and Closed Ports

 

Another important concept you should understand about ports is how to know when a port is open or closed and why. Simply put, an open port is ready to push or pull flow items. A closed port is not ready to push or pull flow items. During a simulation run, open ports are green and red ports are closed:

 

 

In the example in the previous image, the first processor's input port is closed because it is processing a flow item and cannot accept any more items. The second processor's input port is open because it is available to process flow items.
