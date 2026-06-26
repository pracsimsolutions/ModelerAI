---
id: reference_3dobjects_taskexecuters_taskexecuterconcepts_taskexecuterconcepts
name: "Task Executer Concepts"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","TaskExecuterConcepts","TaskExecuterConcepts"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/TaskExecuterConcepts/TaskExecuterConcepts.html"
---

# Task Executer Concepts

## What are Task Executers?

 

Task executer is the top level class for several objects in the library. Operators, Transporters, ASRSvehicles, Cranes and other mobile resources inherit from the task executer class. All of these objects can travel, load flow items, unload flow items, act as shared resources for processing stations, and perform many other simulation tasks.

 

Task executers and their sub-classes are able to: 

- Execute task sequences
- Execute offset travel

 The task executer is also a sub-class of the Dispatcher class, and thus a task executer can actually act as a team leader that dispatches task sequences to other team members. Its handling and dispatching logic have subtle differences from the Dispatcher, however. When the task executer receives a task sequence, it first checks to see if it already has an active task sequence. If there is no active task sequence, or if the newly received task sequence is preempting and has a priority greater than the currently active task sequence, then it will start executing the new task sequence, preempting the active one if needed. Otherwise it will go through the normal Dispatcher logic. If the task sequence is not passed on immediately, then it will queue up in the task executer's task sequence queue, and if the task sequence is still in the queue when the task executer finishes its active task sequence, the task executer will then execute the task sequence.

 

You can view a task executer's active task sequence and task sequence queue during a model run by right-clicking on the task executer and selecting **View Task Sequence Queue.**

 

 

 

## User-Defined Properties

 

All task executers have the following fields that can be defined by the modeler:

 

- **Capacity** - This parameter defines a value for the maximum content of the object. In default operation, the object will never load more flow items than this value specifies.
- **Maximum Speed, Acceleration, Deceleration** - These define the task executer's maximum speed, acceleration, and deceleration. Maximum speed is defined in units of length per unit of time, while acceleration and deceleration are defined in units of length per squared unit of time. If you are defining your model in meters and seconds, for example, the speed value is in m/s, etc. These values are used in defining the object's peak speed and change in speed while executing the task types such as TASKTYPE_TRAVEL, TASKTYPE_TRAVELTOLOC, etc.
- **Travel Offsets for load/unload tasks** - This value determines whether the task executer should execute offset travel to the load/unload location when it loads or unloads a flow item. For example, if this is not checked, and the task executer is traveling on a network, then it will only travel to the network node that is at the load/unload station. It will remain on that node while doing the load.
- **Rotate while traveling** - Here you can specify if you want the object to rotate in the direction that it is traveling. This will have no effect on model output. It is only for visual purposes.
- **Load Time** - See the Load Time event for more information.
- **Unload Time** - See the Unload Time event for more information.
- **Break to Requirement** - This field is executed when the task executer comes to a break task or callsubtasks task. The return value is a reference to a task sequence. The logic within this field should search the task executer's task sequence queue, and find a task sequence that is appropriate to break to.

 

 

 

## Offset Travel

 

Offset travel is a mechanism by which different types of objects can travel differently, yet use the same interface for traveling. For example, an object wants to place an item into a given bay and level of a Rack. The way in which the object travels to the correct location to drop off the item depends on the type of object it is. An operator walks to the bay's location and places the item in the level. A Transporter travels to the bay, but must also lift its fork to the proper height of the level. It can travel in both the x and y direction, but only its fork can travel in the z direction. An ASRSvehicle will only travel along its own x axis, lifting its platform to the height of the level, and then pulling the item from the Rack. Hence, each object implements its travel differently, but the interface is the same: travel to the right spot to place the item in the Rack. Offset travel is essentially the only thing that distinguishes each sub-class of the task executer. For information on how each sub-class implements offset travel, refer to the "Details" section of an object's help page. Offset travel is used in the following tasks: 

- load
- unload
- traveltoloc
- travelrelative
- pickoffset
- placeoffset

 The offset travel interface is very simple. Every type of offset request translates into an x,y, and z offset distance, and sometimes a reference to a flow item. For example, if an object is given a traveltoloc task for the location (5,0,0), and its current location is (4,0,0), then it automatically translates that task into an offset request for (1,0,0), meaning it needs to travel one unit in the x direction. A travelrelative task translates directly. For example, a travelrelative task for (5,0,0) tells the object to travel 5 units in the x direction. Load and unload tasks also use offset travel if the "Travel Offsets for Load/Unload Tasks" checkbox is checked. When an object needs to load a flow item from a station, it queries the station for the location of the item. Also, when it needs to unload an item, it queries the station for the location to unload the item. The station returns an offset x/y/z distance, and the task executer uses this distance to travel its offset. Also, for a load and unload task, the task executer has a reference to the item in its offset request. This may or may not affect the way the object travels, depending on the type of object. For example, the Transporter's offset travel mechanism is implemented so that if there is an item, or in other words, if the Transporter is loading or unloading an item, the Transporter will lift its fork in the z direction. If there is no item, or in other words, if theTransporter is doing a traveltoloc or travelrelative task, then it will actually travel in the z direction, instead of lifting its fork.

 

Offset values should be calculated relative to the x/y center of the object, and the z base of the object. For example, a robot is positioned at location (0,0,0). It has a size of (2,2,1). From this the x/y center and z base can be calculated as the location (1,-1,0) (Note: y size extends along the negative y-axis). All offset calculations should be made from this (1,-1,0) location. While giving the robot a traveltoloc task will automatically make this calculation for you, sometimes it is necessary to calculate this location manually and use a travelrelative task. If the robot is given a travelrelative task of (1,0,0), this means that the correct position to travel to is one unit to the right of the robot's x/y center and z base. This translates to the location (2,-1,0). Note that this does not mean that the robot will travel such that its own location is (2,-1,0). Neither will it travel such that its x/y center and z base are at that location. Because it is a robot, it will rotate and extend its arm so that the end of the arm is at the location (2,-1,0). Its actual location will not change at all. Thus the calculation from the object's x/y center and z base allows you to specify a desired destination location which is the same for all objects, but which allows each type of object to handle that destination location differently.

 

 

 

## Events

 

The following sections explain important considerations related to task executer events. For information on events, see the Event Listening page.

 

### On Entry

 

The OnEntry event fires whenever the TaskExecuter receives a flowitem. This includes when the TaskExecuter loads an item, as well as when the item is moved into the TaskExecuter.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Entering Item | Object | The flow item that entered the object. |
| Input Port | int | The input port number the flow item entered through. |

 

### On Exit

 

The OnExit event fires whenever the TaskExecuter sends a flowitem. This includes when the TaskExecuter unloads an item, as well as when the item is moved out of the TaskExecuter.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Exiting Item | Object | The flow item that is exiting the object. |
| Output Port | int | The output port number flow item will exit through. |

 

### On Load

 

The on load event fires each time a flow item is loaded into the object, after the load time has completed. This event will execute the object's On Load trigger.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Loaded Item | Object | The flow item that was loaded by the object. |
| From Station | Object | The object that the task executer loaded from. |

 

### Load Time

 

The load time event fires at the beginning of each load task. Note that if the task executer is configured to travel offsets for load/unload tasks, then it will first travel the correct offset, and then start the load time. Thus the load time is added onto the end of the offset travel time; it is not part of the offset travel time.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item that will be loaded. |
| Station | Object | The object the task executer will be loading from. |

 

The return value of the load time field is the delay time that the task executer will wait before loading the flow item and moving on to the next task.

 

### On Unload

 

The on unload event fires each time a flow item is unloaded from the object, after the unload time has completed. This event will execute the object's On Unload trigger.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Unloaded Item | Object | The flow item that will be unloaded. |
| To Station | Object | The object that the task executer should unload to. |

 

### Unload Time

 

The unload time event fires at the beginning of each unload task. Note that if the task executer is configured to travel offsets for load/unload tasks, then it will first travel the correct offset, and then start the load time. Thus the load time is added onto the end of the offset travel time; it is not part of the offset travel time. It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item that will be unloaded. |
| Station | Object | The object the task executer will be unloading to. |

 

The return value of the unload time field is the delay time that the task executer will wait before unloading the flow item and moving on to the next task.

 

### On Receive Task Sequence

 

The on receive task sequence event fires each time the task executer receives a new task sequence. This event executes the On Receive Task Sequence trigger.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Task Sequence | treenode | The task sequence the task executer received. |

 

### On Start Task

 

The on start task event fires each time a new task is started by the task executer.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Task Sequence | treenode | The task sequence that the task is part of. |
| Task Rank | int | The rank within the task sequence of the task being started. |
| Task Type | int | The type of the task being started (for example, TASKTYPE_LOAD). |

 

### On Finish Task

 

The on finish task event fires each time a task is completed by the task executer.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Task Sequence | treenode | The task sequence that the task is part of. |
| Task Rank | int | The rank within the task sequence of the task being finished. |
| Task Type | int | The type of the task being finished (for example, TASKTYPE_LOAD). |

 

### On Preempt

 

The on preempt event fires when the task executer is preempted away from its active task sequence to work on another task sequence.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Preempted Task Sequence | treenode | The active task sequence that is being preempted. |
| Task Rank | int | The rank of the task that the task executer was working on when preempted. |
| Task Type | int | The type of the task that the task executer was working on when preempted (for example, TASKTYPE_LOAD). |
| Preempting Task Sequence | treenode | The task sequence that preempted the active task sequence. |

 

### Break To

 

The break to event will only be fired when the task executer performs a "break" task. A standard tasksequence is made up of a travel - load - break - travel - unload sequence of tasks.

 

When the task executer receives a break task, this is a notification that it may now put the currently active task sequence back in its queue and see if there are any other task sequences that it might want to do before it finishes the current one. This allows for capabilities such as loading several items before dropping them off. In such a case, the task executer will first make a check to make sure it is currently capable of "multitasking." This is done by asking "is my content less than my capacity?" If the check is true, then it will execute the break to field to find a task sequence to break to. When finished with new tasks, the task executer will return to the original task sequence.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Active Task Sequence | treenode | The task sequence that the task executer is currently working on when the break task was started. |

 

If the break to returns NULL then the task executer will not break at all. If a valid task sequence is returned, then it will break to the new task sequence and begin executing it.

 

### On Resource Available

 

This event behaves a little differently for a dispatcher and task executer. For a dispatcher, the event is fired whenever a downstream task executer becomes available. For a task executer, the event is fired whenever that task executer finishes a task sequence. If the task executer is also a dispatcher, meaning it has a team that it dispatches tasks to, then the event will be fired for both cases.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Output Port | int | If this event is on a dispatcher, this is the output port number of the available task executer. |
| Resource | Object | The task executer that has become available. |
| Next Task Sequence | treenode | If the task executer has task sequences queued, this will be the next task sequence to be executed. |
| Finished Task Sequence | treenode | The task sequence that was just completed. This task sequence will be destroyed once this event has completed. |

 

If the On Resource Available trigger returns a 0, the dispatcher/task executer will do its own dispatching logic. If the function returns a 1, the dispatcher/task executer will not do anything, and assumes all dispatching logic is done with this trigger using the [TaskSequence.move](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=move)() and [TaskSequence.dispatch](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=dispatch)() methods.

 

### Pass To

 

This function is fired when the Dispatcher receives a task sequence, and should return the output port that the Dispatcher will pass the tasksequence to. If 0 is returned, the tasksequence will automatically queue up according to the defined Queue Strategy until the tasksequence can be passed to an available dispatcher or task executer. If a value greater than 0 is returned, the tasksequence will be sent immediately to the returned port number. If a value of -1 is returned, then the Dispatcher does nothing, but rather assumes all dispatch logic is done within the passto function using the [TaskSequence.move](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=move)() and [TaskSequence.dispatch](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=dispatch)() methods.

 

This event has no parameters.

 

### Queue Strategy

 

This event has no parameters.

 

### On Message

 

The on message event will fire the object's on message trigger. This event is fired when the object receives a message.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Sending Object | Object | The object that sent the message. |
| Message Param 1 | Variant | This is a parameter that was passed through the message and could be a number, string, treenode or array. |
| Message Param 2 | Variant | This is a parameter that was passed through the message and could be a number, string, treenode or array. |
| Message Param 3 | Variant | This is a parameter that was passed through the message and could be a number, string, treenode or array. |

 

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

 

This event fires anytime the object's content changes. This occurs when a flow item is loaded or unloaded from the object.

 

This event has no parameters.

 

### On Input Change

 

This event fires when a flow item is loaded into the object.

 

This event has no parameters.

 

### On Output Change

 

This event fires when a flow item is unloaded from object.

 

This event has no parameters.

 

### On Staytime Change

 

This event fires when a flow item is unloaded from the object, immediately after the On Output Change event.

 

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

 

Below are documented the set of property names supported by task executers.

 

| Property | Type |  |  |
| --- | --- | --- | --- |
| Acceleration | Unit |  |  |
| AvailableOnStart | Boolean |  |  |
| BreakTo | Code | Capacity | Number |
| Deceleration | Unit |  |  |
| DoAnimations | Boolean |  |  |
| ExtensionSpeed | Unit |  |  |
| FlipThreshold | Number |  |  |
| LiftSpeed | Unit |  |  |
| LoadTime | Unit Code |  |  |
| MaxSpeed | Unit |  |  |
| Navigator | Node |  |  |
| OffsetTravel | Options |  |  |
| PassTo | Code |  |  |
| QueueStrategy | Code |  |  |
| RotateTraveling | Boolean |  |  |
| UnloadTime | Unit Code |  |  |

 

 

 

## States

 

The task executer's states are purely dependent on the types of tasks that the task executer performs. Many tasks are associated with a hard-coded state, but with some tasks the modeler can specify an explicit state for the task executer to be in when executing that task. Here are some of the states that you will see often with a task executer.

 

Task executer's states are as follows:

 

### Idle

 

The object is not performing any tasks.

 

### Travel Empty

 

The object is traveling to a destination object and doesn't contain any flow items. This state is exclusively associated with the TASKTYPE_TRAVEL task.

 

### Travel Loaded

 

The object is traveling to a destination object and has load one or more flow items. This state is exclusively associated with the TASKTYPE_TRAVEL task.

 

### Offset Travel Empty

 

The object is performing offset travel and doesn't contain any flow items.

 

### Offset Travel Loaded

 

The object is performing offset travel and has loaded one or more flow items.

 

### Loading

 

The object is loading a flow item. This state corresponds to the TASKTYPE_LOAD task and applies only to the time when the object has finished its offset travel and is waiting its modeler-defined load time before loading the item.

 

### Unloading

 

The object is unloading a flow item. This state corresponds to the TASKTYPE_UNLOAD task and applies only to the time when the object has finished its offset travel and is waiting its modeler-defined unload time before unloading the item.

 

### Utilize

 

The object is being utilized at a station. This state is usually used for an operator when the operator has arrived at the station and is being utilized for a process, setup, or repair time. The utilize state is usually associated with a TASKTYPE_UTILIZE task, but that task can also specify a different state. Other task types, like TASKTYPE_DELAY, can use the utilize state as well.

 

### Blocked

 

The object is currently traveling, but is blocked on the network.

 

 

 

## Statistics

 

The following sections explain important considerations related to task executer statistics.

 

### State

 

A categorical tracked variable is used to collect state history for each object. This data can be used to get the total time at a each state, display the current state of the object on a dashboard or to display the state history for an entire model run. See the Dashboard, Statistics Collector and Tracked Variables pages for more information.

 

### Throughput

 

Throughput is made up of the input statistic and the output statistic. For each flow item that is loaded by the task executer, the input is incremented by one. When the flow item is unloaded, the output is incremented by one. Throughput statistics always increase.

 

### Content

 

The content statistic records how many flow items are inside of the task executer. An object's content can increase or decrease throughout the model run. Additional data can also be gathered from the content statistic to include the minimum, maximum and average value. This data is calculated from the entire model run.

  

#### Container Flow Items

 

A container flow item that contains other flow items will only increase the content of the task executer by one, not by each of the containing flow items.

  

### Staytime

 

The staytime statistic is recorded for each flow item that the task executer unloads. The staytime is equal to the unload time - load time. Additional data can also be gathered from the staytime statistic to include the minimum, maximum and average value. This data is calculated from the entire model run.

 

### Travel Distance

 

The total travel distance of the task executer is recorded as travel tasks are performed.
