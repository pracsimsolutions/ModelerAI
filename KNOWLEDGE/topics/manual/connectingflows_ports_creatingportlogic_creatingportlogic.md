---
id: connectingflows_ports_creatingportlogic_creatingportlogic
name: "Creating Port Logic"
kind: manual
breadcrumb: ["ConnectingFlows","Ports","CreatingPortLogic","CreatingPortLogic"]
tags: ["manual","connectingflows","ports","creatingportlogic"]
source: "manual/ConnectingFlows/Ports/CreatingPortLogic/CreatingPortLogic.html"
---

# Creating Port Logic

## Introduction to Port Logic

 

This topic will explain how to edit the properties on fixed resources in the 3D model to create different types of flow logic. Before reading this topic, make sure you are familiar with the concepts explained in these topics:

 

- Overview of 3D Object Flows
- Key Concepts About Ports

 

You will primarily use the properties under the Input and Output sections of the Properties window to control the flow between fixed resources. You can access these by clicking on a fixed resource to open its properties in the Properties window on the right.

 

 

 

 

 

## Changing the Type of Flow

 

If your simulation model has a fixed resource that is connected to multiple downstream objects, you might want to create logic on your ports that determine which downstream port will receive the outgoing flow item. FlexSim provides a variety of pre-programmed types of flows such as:

 

- **First available** - Flow items will move to the first downstream port that is available to receive the item. (This is the default setting.)
- **Round robin** - Flow items will be sent to each available downstream object in turn. The first flow item will move to the first port, the second flow item will move to the second port, and so on.
- **Random** - Flow items are sent to a downstream port at random, perhaps using a statistical distribution or a distribution defined by percentages.
- **Conditional** - Flow items are sent to a specific port when certain conditions are met or are sent to another port under a different set of conditions.
- **Sequential** - Flow items can be sent to ports based on the steps outlined in a global table.

 

See Common Types of Flows for more detailed explanations and demonstrations of these flows.

 

### Editing the Send to Port Property

 

You will primarily use the **Send to Port** property under the Output section of the Properties window to control how a fixed resource decides which downstream port should receive a flow item. To choose one of the common types of flows:

 

1. Click the upstream fixed resource to open its properties on the right.
1. Under the **Output** section, click the arrow next to the **Send to Port** box.
1. This menu lists many of the common types of flows you could choose. Try experimenting with a few of the options to get comfortable with this property.

 

### About the Send to Port Property

 

If you'll recall from the Key Concepts About Ports topic, each port is assigned a ranking. Every time you create a port connection between two objects, FlexSim automatically assigns a rank to that connection. The first port connection you make will have a rank of 1, the second will have a rank of 2, etc.

 

When a fixed resource is ready to release an item, it will evaluate the Send to Port property and return a port number. Whichever port number it returns will determine which port will receive the flow item.

 

For example, imagine your simulation model has a processor with five output ports to five different queues and you want the processor to send them to a random queue using a statistical distribution. In that scenario, you would set the Send to Port option to random. When the processor finishes processing a flow item, it will call the Send to Port function to determine which port it should send the flow item to. Because the Send to Port property was set to random, the Send to Port function will use a random statistical distribution to return a number between 1 and 5. If it returns the number 3, it will send the flow item to port 3. If it returns a 5, it will send the flow item to port 5, and so forth. See Fixed Resource Concepts for more information.

 

 

 

## Using a Transport

 

If needed, you can use a task executer to transport flow items from one fixed resource to another. (See Using Transports for a more detailed explanation and demonstration.)

 

### The Basic Method for Using a Transport

 

To create flow logic where a fixed resource will use a transport:

 

1. Add at least two fixed resources to the 3D model and connect them using input/output port connections (A-connects). See Adding Ports for more information.
1. Add at least one task executer to the 3D model, possibly between the two fixed resources.
1. Connect the upstream fixed resource to the task executer with a center port connection (S-connect).
1. Click on the upstream fixed resource to open its properties on the right.
1. Under the **Output** section, check the **Use Transport** box. By default, when you check this box, it will use the task executer connected to the fixed resource's first center port.

 

### About the Use Transport Property

 

The Use Transport property tells FlexSim which task executer should transport the flow item. By default, when you check this box, it displays a FlexScript expression that says `current.centerObjects[1]`. What does this expression mean?

 

- The word `current` refers to the current object you are defining the property on. So, if you're defining this property on a processor, it refers to that processor.
- The phrase `centerObjects` is a piece of FlexScript code that tells the object to use the task executer that is connected to its center port.
- The number `[1]` in brackets tells it to use the object connected to the center port with a ranking of 1. If you were to change that number to a 2, it would use the task executer connected to the second port instead.

 

You can change the Use Transport property so that it uses different logic to determine which task executer should be used. If you click the arrow next to the Use Transport box, you'll see a menu that lists several different options for selecting a task executer. Try experimenting with a few of the options to get comfortable with this property.

 

### About Priorities and Preemption

 

You might have noticed that the Use Transport property has two additional properties: Priority and Preemption. These properties control whether the task executer should complete the transportation tasks before or after it works on other tasks.

 

When you create a transport task for a task executer, you're really creating a *task sequence* that has three tasks:

 

1. Load the flow item
1. Travel to the downstream object
1. Unload the flow item

 

It's possible that a task executer might be assigned more than one task sequence at a time. For example, if an operator is transporting flow items for two different processors, it could receive transport task sequences from both processors at the same time.

 

If needed, you can use the Priority property to tell the task executer which task sequence has a higher priority, meaning that the task executer should work on that task sequence before other tasks. In a similar vein, you can use the Preemption property to preempt a task executer, meaning that they will stop the current task they are working on and go work on the more important task sequence.

 

Because the Priority and Preemption properties are important to task sequence logic, these properties will be discussed in the topic about Task Sequences.

 

 

 

## Making an Object Pull

 

If needed, you can make a downstream object pull flow items from an upstream object. For example, you can make a processor pull a flow item based on certain conditions. (See Pushing and Pulling for more information.)

 

### Creating a Pull Strategy

 

To make an object pull from an upstream object:

 

1. Click the downstream object to open its properties on the right.
1. Under the **Input** section, check the **Pull Strategy** checkbox.
1. If you want to change which input ports the fixed resource will pull objects from, click the arrow next to the **Pull Strategy** box to open a menu. This menu lists many of the common types of flows you could choose. Try experimenting with a few of the options to get comfortable with this property.
1. If you want to restrict the criteria for flow items that this fixed resource will pull, click the arrow next to the **Pull Requirement** box to open a menu. This menu lists many different types of flow item restrictions. Try experimenting with a few of the options to get comfortable with this property.

 

### About the Pull Strategy and Pull Requirement Properties

 

The Pull Strategy property controls where the flow item will be pulled. In other words, it determines which input port the fixed resource will pull flow items from.

 

The Pull Requirement property controls what type of flow items can be pulled from the upstream fixed resource. For example, you could require the fixed resource to only pull flow items with a particular item type or a matching label.

 

When the fixed resource is ready to pull an item, FlexSim will evaluate the Pull Strategy to determine which port it should pull from. Then it will evaluate whether there is a flow item that matches its criteria or restrictions. It will attempt to pull from this port until a flow item matches its criteria.

 

 

 

## Changing Port Rankings

 

To change an object's port rankings:

 

1. Click the object to open its properties on the right.
1. Under the **Ports** section:
1. The top box lists the different types of port connections. (See Types of Port Connections for more information.) Select the type of port connections you want to re-rank.
1. The list box below displays all of the ports of that type that are connected to this object. The ports are ranked with numbers and are listed in the order they are ranked. Click the port you want to re-rank to select it.
1. Use the buttons on the right to rank the port up or down.
