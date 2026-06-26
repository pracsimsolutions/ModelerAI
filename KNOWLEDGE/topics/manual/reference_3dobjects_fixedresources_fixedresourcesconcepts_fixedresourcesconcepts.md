---
id: reference_3dobjects_fixedresources_fixedresourcesconcepts_fixedresourcesconcepts
name: "Fixed Resources Concepts"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","FixedResourcesConcepts","FixedResourcesConcepts"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/FixedResourcesConcepts/FixedResourcesConcepts.html"
---

# Fixed Resources Concepts

## What are Fixed Resources?

 

Fixed resources are objects that remain "fixed" or stationary in your model. Generally speaking, fixed resources interact with flow items in the business system in various ways, such as storing or modifying flow items. Fixed resources can represent various steps or processes in your model, such as processing stations or storage areas.

 

Fixed resources are a superclass of the following 3D objects:

 

- Source
- Queue
- Processor
- Sink
- Combiner
- Separator
- MultiProcessor
- Storage Object

 

This topic will give in-depth information about how fixed resources work. It is intended for advanced users who want to get a deeper understanding of how fixed resources function.

 

 

 

## Fixed Resource Logic

 

All fixed resource have similar logic for pulling flow items into the station, as well as sending the object on. They receive flow items through their input ports, do something to those flow items, then release the flow items to be sent on through their output ports.

 

While different types of fixed resources receive and release flow items at different times, the logic for receiving and releasing a flow item are the same for all fixed resources. For example, the Queue can receive several flow items at the same time. The Queue also releases each flow item as soon as it enters the Queue. The Processor on the other hand receives exactly one flow item, processes that flow item, then releases it and waits until the flow item has left before receiving the next flow item. Although the Queue and Processor receive and release flow items at different times, the processes of receiving and releasing the flow item are the same for both. Each goes through a certain set of steps for each flow item that it receives and releases. Some of these steps are automatically handled by the fixed resource, and some allow you as a modeler to define the way flow items are received and released. All of these modeler-defined inputs can be edited in the Input and Output sections of an object's Properties. The following diagram shows the steps that a fixed resource object goes through for each flow item that it receives and subsequently releases.

 

 

The following steps explain the logic of each of the major steps in the previous diagram:

 

### 1. Open input ports and find a flow item to receive

 

When the fixed resource becomes ready to receive a flow item, it checks first to see if it is in **Pull** mode. If it is in Pull mode, then it calls the Pull Strategy function. This function returns a value of the input port number to open. If 0 is returned, then it will open all input ports. When an upstream item is released, it calls the Pull Requirement field for that item. This field should return a true (1) or false (0). If true, then it receives the flow item. If false, then it tries calling the Pull Requirement function again for the next flow item that has been released, or waits until another flow item is released by an upstream fixed resource. It continues this loop until the Pull Requirement returns a yes (1) for a particular item, upon which it receives that item. If the object is not in Pull mode, however, then the fixed resource skips all of the pulling logic, and simply waits for the first flow item that becomes available.

 

#### Pull Logic Example

 

 

The above diagram shows two fixed Resources. The pulling fixed resource is set to pull from the upstream fixed resource. The upstream fixed resource has released 3 of its flow items (green), while 2 flow items are still being processed (red). When the pulling fixed resource is ready to receive one of the upstream fixed resource's flow items, it calls its Pull Requirement function for each of the 3 released flow items, until the Pull Requirement function returns a yes (1). As soon as a yes is returned, the pulling fixed resource will receive that item and finish its pulling logic, until it is ready to receive the next flow item. If all of the 3 calls to Pull Requirement return a no (0), then the pulling fixed resource will wait. When flow item 4 is released later in the simulation, the pulling fixed resource will then call Pull Requirement for item 4, and if it returns yes, item 4 will be received. If it returns no, this process will repeat until a flow item is found that meets the Pull Requirement.

  

#### Waiting for an Upstream Flow Item to Be Released

 

When an upstream flow item is released, the pulling fixed resource will only call the Pull Requirement on that specific flow item. It will not call the Pull Requirement on previously released flow items for which the Pull Requirement has already returned a no (0).

  

After a flow item is selected, received, and released the pulling fixed resource is again ready to recieve one of the upstream fixed resource's flow items. The same process is repeated such that all the flow items are reevaluated, including the flow items that returned a no (0) before.

  

#### Send to Port of Upstream Objects

 

In FlexSim versions before 6: if an object was configured to pull from upstream objects, the Send To Port of those objects was nullified. In version 6, the Send To Port of upstream objects is evaluated in conjunction with the pull strategy. Both the send-to-port and the pull strategy must be true for the item to be pulled.

  

### 2. Process the flow item

 

Once the flow item has entered the fixed resource, the item is "processed" according to the type of fixed resource, and then released. For example, if it is a Processor object, then the flow item will be processed for a certain amount of time. If it is a Queue object, then the product is released immediately.

 

### 3. Release the flow item and determine which output ports to open

 

When the flow item is released, the fixed resource calls the Send To Port function. Like the Pull from Port function, this function returns a port number. The fixed resource then opens that port. If the port number is 0, then it will open all of its output ports. Once the ports are opened, the fixed resource waits until a downstream object becomes ready to receive the flow item. If the fixed resource is configured to reevaluate its Send To Port, then each time a downstream fixed resource becomes available to receive a new flow item, the upstream fixed resource will reevaluate the Send To Port for that flow item. It's important to note here that this is only executed when the downstream object becomes available. It does not continuously evaluate just because a downstream object is already available. If you want to manually force a re-evaluation at some other time than when a downstream object becomes available, then you can do so by calling the openoutput() command on the upstream object.

  

#### Send to Port Value

 

If the returned port is greater than 0, then that port will be opened. If the returned port is 0, then all ports will be opened. If the returned port is -1, the flow item will not be released, and should be released explicitly later on using the releaseitem() command, or should be moved out using the moveobject command. When it is released again, the Send To Port function will be called again. A -1 return value is more for advanced users.

  

### 4. Transfer the flow item to the next station

 

Once the flow item is released, and a downstream object is ready to receive it, if the "Use Transport" checkbox is not checked, then the item will be passed immediately in to the downstream object. If the "Use Transport" checkbox is checked, then the fixed resource will call the Request Transport from function. This function should return a reference to a TaskExecuter or Dispatcher. If a valid reference is returned, then a default task sequence will automatically be created, and a TaskExecuter will eventually pick the flow item up and take it to its destination. You can also return a 0 value in the Request Transport From field. If a 0 value is returned, then the fixed resource will assume that a task sequence has been created explicitly by the user, and will not create the default task sequence himself. If you return a zero, then you will need to create the task sequence yourself. You can easily get started at doing this by selecting the "Create task sequence manually" pick option in the Request Transport Field picklist, then manually editing the code from there.

 

 

 

## Using a Transport

 

If an object is configured to use a transport to transport flow items to downstream objects, then when the downstream object pulls the flow item or becomes ready to receive the flow item, instead of immediately moving the flow item into the next station, the object creates a task sequence for a Task Executer to travel to the object, pick up the flow item, travel to the downstream object, and drop it off there. This operation involves several important steps. First, when this happens, the object calls its Request Transport From function, and gets a reference of the object to give the task sequence to. Then the flow item goes into a "Waiting For Transport" state. This means that the destination for that flow item has been set in stone, and cannot be changed. Send To Port and pull screening has already finished and decided on sending the flow item out that port, and this decision will not change. Also, each fixed resource object keeps track of two numbers: the number of flow items that are in transit to the object, and the number of flow items that will be transported out of the object, but have not been picked up yet. These two variables are respectively called nroftransportsin and nroftransportsout. Once the object has called the Request Transport From field, it increments its own nroftransportsout variable, and notifies the downstream object, which subsequently increments its own nroftransportsin variable. The object then creates a task sequence of:

 

1. Travel to the upstream object: Travel task.
1. Load the item: FRLoad task.
1. Break to other task sequences if appropriate: Break task.
1. Travel to the downstream object: Travel task.
1. Unload the item into the downstream object: FRUnload task.

  

#### Frload/Frunload Tasks

 

The fixed resource uses frload/frunload tasks instead of regular load/unload tasks. These tasks are just like regular load and unload tasks except that right before the TaskExecuter moves the flow item, it notifies the fixed resource of the operation being done, so that the fixed resource can appropriately decrement its nroftransportsin/nroftransportsout variable. In summary, the nroftransportsout of the upstream object and the nroftransportsin variable of the downstream object are both incremented at the same time that the Request Transport From function is called and the task sequence is created. The nroftransportsout variable is then decremented just before the TaskExecuter finishes the frload tasks and moves the flow item out of the upstream object. The nroftransportsin variable is decremented just before the TaskExecuter finishes an frunload task and moves the flow item into the downstream object.

  

The integrity of the nroftransportsin and nroftransportsout variables is essential to the correct operation of the objects involved because each object may screen further input/output based on these variables. For example, consider a queue with capacity of 10. If the queue's current content is 5 and it also has 5 products that have not arrived yet, but are in transit to the queue, then the queue must close its input ports because it may possibly become full, even though at this point it only has 5 products. An incorrect nroftransportsin variable could cause serious problems for the queue's content. What does this mean for you? Two things. First, if an object has chosen to transport a flow item to a given downstream object, there is no turning back or redirecting the flow item to a different destination, because this would mess up the proper decrementing of nroftransportsin/out variables. Secondly, be very aware of when you use frload/frunload versus regular load/unload, because this can affect the input/output functionality of the objects. Simply put, each execution of a Request Transport From function should eventually (if not immediately) result in the creation of exactly one frload task to load the item from the upstream object and exactly one frunload task to unload the item to the downstream object. In all other cases, regular load and unload tasks should be used.

 

 

 

## Alternatives to Using Port Connections

 

You can control the flow of items between fixed resources using a few different methods:

 

- Use standard port connections and standard send-to/pull logic as explained in the previous sections.
- Push and pull from a list, instead of connecting output/input ports.
- Initiate transport manually, bypassing port hand-shaking logic.

 

The following sections describe these scenarios.

 

### Using Standard Port Connections

 

It's important to first explain what the standard port connection mechanism is doing.

 

1. **Ports define the objects a fixed resource can send to and pull from** When you connect fixed resource input and output ports together, you are defining the search patterns by which upstream fixed resources can find downstream fixed resources to send their items to, and/or the search patterns by which downstream fixed resources can find items to pull from upstream fixed resources. For example, when a downstream fixed resource is ready to receive its next item, it is limited in its search for receivable items to the objects that are connected to its input ports. Also it will search them in the order that the ports are connected (input port 1 first, then port 2, and so forth).
1. **Ports contain open/closed state to determine availability** Input and output ports can be open or closed. Fixed resources use this open/closed state in deciding where items can go. When an item is released to port 1, for example, the object's output port 1 is made open (green). If the corresponding input port of the downstream object is also open (green), then that is a signal that the downstream object is ready to receive an item, so the item will be sent to that downstream object.
1. **Port rankings can be important** If you are use routing rules based on defined values, i.e. itemtype or labels, port rankings can be important. For example, if you're sending by itemtype directly, then items with itemtype 1 will be sent to port 1. This means you must make sure your port rankings are ordered correctly.

 

There are some situations that work well using port connections:

 

- **One-to-One Routing** - Port connections work well when you're just ending an item from one station to a single next station. Just connect the upstream station to the downstream station with an 'A' connection.
- **One-to-Many Routing** - Using port connections works well if you're sending an item from one station to one of many stations using a basic routing rule like round-robin, first available, random, or by some defined value like itemtype. Just connect the station to the set of downstream stations, then define the routing rule through the Send To Port field in the object's Flow tab.
- **Many-to-One Routing** - Using port connections can work well if you're send an item from one of many stations to a single downstream station using a basic pulling rule like round-robin, first available, longest waiting, random, or by some defined value like item type. Just connect each upstream station to the downstream station, then define the pulling rule through the Pull Strategy field in the object's Flow tab.

 

### Pushing and Pulling from a List

 

Some scenarios can make the management of port connections difficult, such as many-to-many routing, or when prioritization is complex. In these cases, using Lists to define the search pattern for sending and receiving items can be easier than using ports. The connectionless routing example shows how to do this.

 

In this example you use pick list options that push and pull from an item list. These pick list options override the default port connection mechanism. Here the upstream objects' Send To Port logic pushes items onto the list, while downstream objects' Pull Strategy pulls items from the list. The search pattern is thus defined by the items that are on the list at any given time, i.e. the items that have been released by upstream objects. Since the searching relies solely on the list, connecting ports is no longer necessary, so you can leave objects that use this mechanism unconnected.

 

### Initiating Transport Manually

 

In some cases, you may want to initiate transport of items before the downstream object(s) are ready to receive them. As explained above, when using standard send-to/pull logic, transport defined by the Use Transport checkbox will only be initiated when the downstream object is ready to receive the item. To override this functionality you would need to bypass the send-to/pull mechanism altogether. To do this, leave the upstream and downstream object input and output ports unconnected. Then use triggers on the upstream object(s), such as OnEntry or OnProcessFinish, to manually create a task sequence to transport the items to their destinations. If you are using the Process Flow module, you could use an Event-Triggered Source to start a custom process when the desired event happens. You can use center ports for referencing between objects.

 

 

 

## Events

 

The following sections explain important considerations related to fixed resource events. For information on events, see the Event Listening page.

 

### On Entry

 

The on entry event will fire the object's on entry trigger. This event fires as soon as an item enters the object, before any other logic is executed. This means that you can change the object's variables, labels, etc., and have those changes be applied correctly within the event logic. However, executing commands that may affect further events of the object should not be executed in the entry/exit trigger, because some events have yet to be created in the event logic of the object, and functions which affect the object's events should wait until those events have been created. In such a case you should send the object a delayed message in 0 time (using the senddelayedmessage() command), and then execute the functionality from the message trigger. This allows the object to finish the rest of its event logic before your commands are executed. These include (but are not limited to), stopping an object, opening and closing input and output ports and in some cases the creating and dispatching of task sequences, depending on the types of tasks that are in them.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Entering Item | Object | The flow item that entered the object. |
| Input Port | int | The input port number the flow item entered through. |

 

### On Exit

 

The on exit event will fire the object's on exit trigger. This event fires as an item is leaving the object, before it has actually left the object. This means if you check the content of the object in the on exit trigger, the content will include the exiting item. As with the On Entry event, you should use the senddelayedmessage() command when execution certain functionality lie stopping the object or opening and closing ports.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Exiting Item | Object | The flow item that is exiting the object. |
| Output Port | int | The output port number flow item will exit through. |

 

### On Message

 

The on message event will fire the object's on message trigger. This event is fired when the object receives a message.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Sending Object | Object | The object that sent the message. |
| Message Param 1 | Variant | This is a parameter that was passed through the message and could be a number, string, treenode or array. |
| Message Param 2 | Variant | This is a parameter that was passed through the message and could be a number, string, treenode or array. |
| Message Param 3 | Variant | This is a parameter that was passed through the message and could be a number, string, treenode or array. |

 

### Pull Requirement

 

The pull requirement event only fires if the object's **Pull** is enabled. This event fires whenever an input port becomes ready. An input port is ready when it is open AND the upstream output port that it is connected to is open. The connection becomes ready when either the input port is opened, OR the upstream output port is opened (assuming the paired port is already open). The pull requirement will be evaluated for each ready flow item within the object connected to the input port which just became ready.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The ready flow item that is being checked to see if it should be pulled or not. |
| Port | int | The port number that the ready flow item is trying to enter through. |

 

The object's pull requirement field must return either true (1) or false (0). If the expression returns a true (1), then the ready flow item will be pulled in through the ready input port. If the expression returns false (0), then pull requirement will be evaluated for the next ready flow item.

 

### Pull Strategy

 

The pull strategy event only fires if the object's **Pull** is enabled. The object's pull strategy field should return the port number to pull flow items from. Returning 0 will pull from the first available port. This event fires when an input port becomes ready. See the Pull Requirement event above for what constitutes a ready port.

 

This event has no parameters.

 

### Send to Port

 

This event is fired once for each flow item at the time the flow item is ready to be sent to the next object. The time at which the flow item is ready will vary depending on the object. For example, a Processor is ready to send a flow item at the end of its processing time. A Queue is ready to send a flow item after its batch has accumulated and has been released.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Releasing Item | Object | The flow item to release. |

 

If the return value of the send to port field is 0, the object will release the flow item to the first available downstream object. This will cause all output ports to be opened, and the flow item may leave through the first ready port.

 

You can return the port number of a connected out object to release the item to a specific object. This will cause the output port to be opened, and the port number will be assigned to the flow item. The flow item will then be pushed (or pulled) when the port connection becomes ready (output port and connected downstream input port are open).

 

If the return value is a -1, then the flow item will not be released at all, and should be released later on using the releaseitem() command, or should be moved out using the moveobject() command.

 

### Transport Reference

 

The transport reference event allows you to define which task executer object will be used to transport the releasing flow item to a downstream object. This event is only fired if the **Use Transport** box is check on the object's properties under the Output section. This event fires after the send to port event if the downstream object is available to receive the releasing item.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item to transport. |
| Port | int | The port number the flow item will exit through as specified by the Send To Port field. |
| Destination | Object | The destination object the flow item will be transported to. |
| Priority | double | The priority of the transport request as set by the object's properties. |
| Preempt | int | The preemption option as set by the object's properties. |

 

The return value of the transport reference field is the reference to the task executer object that will be given the task sequence for transporting the releasing item. If the return value is 0, it is assumed that you created and dispatched your own task sequence in the transport reference field. In this case, the object will not dispatch its own task sequence.

 

### Pick Offset

 

This event fires when the transporting object has moved to the fixed resource object and is ready to use offset travel in order to travel to the item's position to pick the item up.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The item that is being transported from the fixed resource object. |
| Loader | Object | The task executer object that is loading the item at the fixed resource object. |

 

### Place Offset

 

This event fires when the transporting object has moved to the fixed resource object and is ready to use offset travel in order to travel to the destination where the item will be placed.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The item that is being transported to the fixed resource object. |
| Unloader | Object | The task executer object that is unloading the item at the fixed resource object. |

 

### On Stop

 

This event fires when the object is stopped either through code using `Object.stop()` or by using a Time Table or MTBF/MTTR tool.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| # Stop Requests | int | The number of stop requests the object has. This number includes the current stop request. |
| State | int | The state the object will go into it while stopped. |
| ID | int | The id or key for the stop request. |
| Priority | double | The priority of the stop request. |
| State Profile | int | The profile number of the new state. The default state profile is 0. |

 

### On Resume

 

This event fires when the object is resumed either through code using `Object.resume()` or by using a Time Table or MTBF/MTTR tool.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| # Stop Requests | int | The number of stop requests the object has remaining. |
| ID | int | The id or key for the resume request. |
| State Profile | int | The profile number of the state that was used when the object was stopped. The default state profile is 0. |

 

### On Content Change

 

This event fires anytime the object's content changes. This occurs when a flow item enters or exits the object.

 

This event has no parameters.

 

### On Input Change

 

This event fires when a flow item enters the object.

 

This event has no parameters.

 

### On Output Change

 

This event fires when a flow item exits the object.

 

This event has no parameters.

 

### On Staytime Change

 

This event fires when a flow item exits the object, immediately after the On Output Change event.

 

This event has no parameters.

 

### On State Change

 

This event fires when the object's state changes. This event allows you to listen to the object's state being changed to a specific state, or to track the state's the object is in.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| State Profile Node | treenode | If the To State is in the default state profile, this parameter is NULL. Otherwise, this parameter is a reference to the state profile node (tracked variable) in the object's state_profiles attribute node. |
| To State | int | The state number the object will be set to. |
| From State | int | The state number the object was in before the new state is set. |

 

### On State Value Change

 

The on state value change event is associated with the tracked variable that stores the state statistics for the object. Listening to this event allows you to use the change rule. This event fires at the same time as the on state change event.

 

This event has one requirement, the state profile number. If using the default state profile, this number is 0.

 

### On Reset

 

The on reset event is fired for each object when the model is reset.

 

This event has no parameters.

 

### On Draw

 

The on draw event is fired each time the 3D view is repainted. This happens at different times depending on run speed and refresh rates. In order to ensure model repeatability, code used in the on draw trigger should not use the stochastics commands (distributions) that use FlexSim's random number generator.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| View | treenode | The view the object is being drawn in. Having multiple 3D views open will cause the on draw event to be fired for each view. |

 

 

 

## Properties

 

Below are the set of property names supported by fixed resources.

 

| Property | Type |
| --- | --- |
| MaxContent | Number |
| Pull | Boolean |
| PullRequirement | Code |
| PullStrategy | Code |
| SendToPort | Code |
| TransportPreempt | Options |
| TransportPriority | Number |
| TransportRef | Code |
| UseTransport | Boolean |

 

 

 

## States

 

For information about collecting state statistics see the Statistics section below. Each fixed resource implements its own set of states. For information on a fixed resource's states, see the reference page for that fixed resource.

 

 

 

## Statistics

 

The following sections explain important considerations related to fixed resource statistics.

 

### State

 

A categorical tracked variable is used to collect state history for each object. This data can be used to get the total time at a each state, display the current state of the object on a dashboard or to display the state history for an entire model run. See the Dashboard, Statistics Collector and Tracked Variables pages for more information.

 

### Throughput

 

Throughput is made up of the input statistic and the output statistic. For each flow item that enters the fixed resource, the input is incremented by one. When the flow item exits the fixed resource, the output is incremented by one. Throughout statistics always increase.

 

### Content

 

The content statistic records how many flow items are inside of the fixed resource. An object's content can increase or decrease throughout the model run. Additional data can also be gathered from the content statistic to include the minimum, maximum and average value. This data is calculated from the entire model run.

  

#### Container Flow Items

 

A container flow item that contains other flow items will only increase the content of the fixed resource by one, not by each of the containing flow items.

  

### Staytime

 

The staytime statistic is recorded for each flow item that exits the fixed resource. The staytime is equal to the exit time - entry time. Additional data can also be gathered from the staytime statistic to include the minimum, maximum and average value. This data is calculated from the entire model run.
