---
id: reference_3dobjects_fixedresources_combiner_combiner
name: "Combiner"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","Combiner","Combiner"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/Combiner/Combiner.html"
---

# Combiner

## Overview and Key Concepts

 

The combiner is used to group multiple flow items together as they travel through the model. It can either join the flow items together permanently, or it can pack them so that they can be separated at a later point in time. The combiner will first accept a single flow item through input port number 1 before it will accepts the subsequent flow items through the remaining input ports. The user specifies the quantity of subsequent flow items to accept through input ports 2 and higher. Only after all subsequent flow items required by the user have arrived will the setup/process time begin. The combiner can be set to require operators during its setup, processing and repair times.

 

 

The combiner is a sub-class of the processor, which is in turn a fixed resource. During operation, the combiner first receives exactly one flow item from its first input port. It will wait until a flow item has arrived through input port 1 before it allows other flow items in. Then it collects a batch of flow items using its component list. The component list specifies the number of flow items the combiner should receive from each other input port for each batch. Row 1 of the component list corresponds to the number of flow items that should be received from input port 2. Row 2 corresponds to input port 3, and so forth. The component list is updated automatically when you connect objects to its input ports.

 

Once the combiner has collected a batch, it goes through a setup and process time, calling operators to the setup and process operations as defined by the processor functionality. See Processor for more information.

 

The combiner can operate in one of three modes: pack, join, or batch. In pack mode the combiner moves all flow items that were received through ports 2 and higher into the flow item received through input port 1 and then releases this container flow item. In join mode the combiner destroys all flow items except the one received through input port 1. In batch mode, the combiner simply releases all the flow items once the batch is collected and the setup and process times have finished.

 

The Pull Strategy field is not used for the combiner. The combiner handles this logic on its own.

 

If you are transporting flow items into the combiner, then while it is receiving the container flow item, it will only allow one flow item to be in transit to itself at a given time, namely the container flow item. Once the container flow item has arrived, the combiner will allow all other flow items for the components list to be in transit at the same time.

  

#### Receiving More Than One Flow Item Through Input Port 1

 

The combiner is configured to always receive exactly one flow item from input port 1. If you are in batch or join mode, you may want to receive more than one flow item from the upstream object that is connected to input port 1. Here you can do one of two things. The simplest option is to connect the upstream object to both input ports 1 and 2 of the combiner, then in the components list make the first row entry be one less than the number of flow items you want to collect. The combiner will receive one flow item through input port 1 and then the remaining number you want from input port 2. If this doesn't work in your scenario, then the other option is to add a source to your model, connect it to input port 1 of the combiner, and give the source a constant inter-arrival time of 0.

  

### Receiving Different Types of Flow Items

 

If you have a single upstream object that can hold many different types of flow items, but you want to screen these different types separately in the combiner's component list, you can do this by connecting several output ports of the upstream object to several input ports of the Combiner. For example, a combiner receives item types 1 and 2 from a single upstream processor. You want to collect 4 of item type 1, and 6 of item type 2, and pack them onto a pallet.

 

To do this, first connect the pallet's source to input port 1 of the combiner. Then connect the processor's output port 1 to the combiner's input port 2, and then connect the Processor's output port 2 to the combiner's input port 3. Have the processor's sendto strategy send by item type. Then in the combiner's component list, enter a 4 in the row corresponding to input port 2, and a 6 corresponding to input port 3.

 

### Manually Moving Flow Items Out of the Combiner

 

If you manually move the container flow item out of the combiner, either using a task sequence or the moveobject command, make sure that you specify a non-zero port number for the item to exit through. When the combiner is packing, it moves packed flow items out of itself into the container flow item. This causes its exit trigger to fire, and the way that it distinguishes between a part moving into its container and the container exiting is by the port number of the exit trigger. If the port number is 0, it assumes it is a part being moved into the container, and does nothing. Thus, if you explicitly move the container flow item out of the combiner, and the port number is 0, it will assume it is a packed flow item, and will not receive the next container flow item.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The combiner uses all of the same events as the processor. See Processor - Events for an explanation of these events.

 

 

 

## States

 

The combiner uses all of the same states as the processor. See Processor - States for an explanation of these states.

 

 

 

## Statistics

 

The combiner uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties

 

The Combiner object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Combiner
- Processor
- Output
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The Combiner object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Combiner object uses the following properties:

 

| Property | Type |
| --- | --- |
| CombineMode | Options |
| ComponentsList |  |
