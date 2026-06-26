---
id: reference_3dobjects_fixedresources_basicfr_basicfr
name: "Basic FR"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","BasicFR","BasicFR"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/BasicFR/BasicFR.html"
---

# Basic FR

## Overview and Key Concepts

 

The BasicFR object is a fixed resource (FR stands for *fixed resource*) that is designed to be customized into a user library object. It passes almost all inheritable fixed resource logic to pick-list functions so that user library developers can specify virtually all fixed resource functionality for the object.

 

 

The BasicFR allows you to specify logic for its reset, entry, exit, and message triggers, as well as advanced functionality like stop object/resume object, pick/place offset, transport in notify/complete, transport out notify/complete, and other advanced functions.

 

In the entry, exit, reset, and message triggers of this object, you will need to implement logic that receives and releases flowitems using the receiveitem() and releaseitem() commands. There are also several more commands that you can use in processing items, such as setstate(), senddelayedmessage(), and all of the commands in the fixed resource category of the command list. This object is meant to be a bare-bones implementation of the fixed resource, where all logic is implemented by the modeller.

 

### Commands

 

`receiveitem()`

 

`releaseitem()`

 

### Extra Parameters for the Entry/Exit Triggers

 

The BasicFR passes 2 extra parameters into its entry and exit triggers:

 

- **param(3)** - the current value of its nroftransportsin variable.
- **parval(4)** - the current value of its nroftransportsout variable.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The BasicFR uses the standard events that are common to all fixed resources. See Fixed Resources - Events for an explanation of these events.

 

The BasicFR has the following additional events:

 

### Advanced Functions

 

This event is fired for various reasons. For the most part, you will not need to implement any logic for these notifications, but they are nonetheless made accessible to you. When the event is fired it will execute the Advanced Functions trigger.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Message Type | int | The type of the notification. See below for more information. |

 

The different message types are listed as follows:

 

- **ADV_FUNC_CLASSTYPE** - This is a request to get the type of class of the object. The classtype should be returned as an integer with certain bits set high. You can construct this value using the bitwise OR operator | and several classtype macros. For example, a FixedResource's classtype is: CLASSTYPE_FLEXSIMOBJECT > CLASSTYPE_FIXEDRESOURCE.
- **ADV_FUNC_DRAGCONNECTION** - This function is called when a keyboard key is held down, and the user clicks and drags from this object to another object. In this case, the object to which the mouse was dragged is passed in as param(2), the ascii value of the key that was clicked is passed in as param(3), and the classtype value of the object is passed in as param(4).
- **ADV_FUNC_CLICK** - This function is called when the object is clicked on. Here a reference to the view in which it was clicked is passed in as param(2), and the click code is passed in as param(3). Possible click codes are: DOUBLE_CLICK, LEFT_PRESS, LEFT_RELEASE, RIGHT_PRESS, RIGHT_RELEASE.
- **ADV_FUNC_KEYEDCLICK** - This function is called when a key on the keyboard is held down and the object is clicked on. Here the view is passing as param(2), the click code is passed in as param(3), and the ascii value of the pressed key is passed in as param(4). Possible click codes are: DOUBLE_CLICK, LEFT_PRESS, LEFT_RELEASE, RIGHT_PRESS, RIGHT_RELEASE.

 

The return value of the Advanced Functions trigger should either be a 1 or a 0. If 0, the object will execute the default functionality associated with the given notification. If 1, the object not do any default functionality, but assumes that the function has overridden the default.

 

### On State Change

 

This event is fired when the object's state is changed with the `Object.setState()` command.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| State | int | The new state the BasicFR is changing to. |
| State Profile | int | The profile number of the new state. The default state profile is 0. |

 

If the function returns a 0, the object will proceed to change its state. If the function returns -1, the object will do nothing, leaving it in its previous state. If the function returns a positive number, the state will be changed to this new state number.

 

### On Transport Out Notify Complete

 

This event is fired at two different times. First is when the object is notified that a flow item is going to exit the object using a transport. This function is referred to as transport out notify. The second time it is fired is when the transport has arrived, finished its load time, and is about to move the flow item out. This is called transport out complete. A variable is passed into this function telling you which operation is applicable. In this field you can manage things like the nroftransportsout variable, as well as how to screen further output/input to the object.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Operation | int | This is 1 or 0. 1 means it is a transport out notify operation, 0 means it is a transport out complete operation. |
| Item | Object | The item that is going to leave this object. |
| Port | int | The output port number through which the item will exit. |
| Transporter | Object | For a transport out complete operation, this is the task executer that is picking the item up. |
| Num Transports Out Node | treenode | The value of this node should be incremented by 1 at a notify operation, and decremented by 1 at a complete operation. You can also get the value of this node to know how many items are waiting for a transport to pick them up. |

 

If the return value is 0, the object will assume that nothing was done and will execute its own default logic. If this function returns a 1, the object will assume the proper variable management was done and will do nothing. If this function returns a -1 and the operation is a complete operation, the object will again assume proper variable management, but in addition, it will notify the transporter that it is not ready to receive the flow item. The transporter then must wait until it is notified that it can resume its operation. The reason you may need to use this is in case this object has been stopped using Object.stop(). If so, you may not want any flowitems going out. If this is the case, then you will need to save off a reference to the transporter using the savestoppedtransportout() function, and then return -1. Then, when it is ok for the transporter to resume its operation (usually from this object's resume object function) you will need to call resumetransportsout() to notify all stopped transports that they may resume their operation.

 

### On Transport In Notify Complete

 

This event is fired at two different times. First is when the object is notified that a flow item is going to enter the object using a transport. This function is referred to as transport in notify. The second time it is fired is when the transport has arrived, finished its unload time, and is about to move the flow item into the object. This is called transport in complete. A variable is passed into this function telling you which operation is applicable. In this field you can manage things like the nroftransportsin variable, as well as how to screen further output/input to the object. For example, you may want to allow more than one flow item to be in transit to the object at the same time. In such a case, you could call receiveitem when you are notified of an upcoming entry within this field.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Operation | int | This is 1 or 0. 1 means it is a transport in notify operation, 0 means it is a transport in complete operation. |
| Item | Object | The item that is going to enter this object. |
| Port | int | The input port number through which the item will enter. |
| Transporter | Object | For a transport in complete operation, this is the task executer that is dropping the item off. |
| Num Transports Out Node | treenode | The value of this node should be incremented by 1 at a notify operation, and decremented by 1 at a complete operation. You can also get the value of this node to know how many items are still in transit to this object. |

 

If the return value is 0, the object will assume that nothing was done and will execute its own default logic. If this function returns a 1, the object will assume the proper variable management was done and will do nothing. If this function returns a -1 and the operation is a complete operation, the object will again assume proper variable management, but in addition, it will notify the transporter that it is not ready to receive the flow item. The transporter then must wait until it is notified that it can resume its operation. The reason you may need to use this is in case this object has been stopped using Object.stop(). If so, you may not want any flowitems coming in. If this is the case, then you will need to save off a reference to the transporter using the savestoppedtransportin() function, and then return -1. Then, when it is ok for the transporter to resume its operation (usually from this object's resume object function) you will need to call resumetransportsin() to notify all stopped transports that they may resume their operation.

 

### Pick Place Offset

 

The pick place offset event is fired at the same time as the task executer standard events, pick offset and place offset. The difference is in what parameters are available. For place offset, this event is fired when a transport object is attempting to place, or unload, a flow item into this object. For pick offset, this event is fired when a transport is about to pick, or load, an item from this object.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Pick Operation | int | This is either a 1 or 0. 1 means it is a pick operation, 0 means it is a place operation. |
| Item | Object | The item that is being picked or placed. |
| Other Object | Object | The object that is picking or placing the item. |
| X Value Node | treenode | The value of this node should be set in this function, and will represent the offset location returned by this function. For example, if I want the picking/placing object to offset 10 in the x direction, 0 in the y direction, and 5 in the z direction, I would set the X Value Node to 10 using xvalnode.value = 10, yvalnode.value = 0, and zvalnode.value = 5. |
| Y Value Node | treenode | See X Value Node. |
| Z Value Node | treenode | See X Value Node. |

 

For both operations, if you are implementing your own pick/place logic here and do not want the default logic to be executed, you must return 1 from this function. If a 0 is returned the object will assume that nothing was done and will execute its own default logic.

 

### Stop Resume

 

The stop resume event is fired at the same time as the task executer standard events, on stop and on resume. The difference is in what parameters are available. For stop, this event will fire when the object is stopped through the Object.stop() command. For resume, this event will fire when the Object.resume() command is called. This event will execute the Stop Object - Resume Object trigger. This trigger should define a strategy for how the object will "stop", and how it will "resume". This field should also manage data for remembering how many stop requests have been made, what the state of the object was before it was stopped, etc.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Stop Operation | int | This value is either a 1 or 0. 1 means the object is being stopped, 0 the object is being resume. |
| Stop State | int | This value is only applicable for a stop operation. It specifies the requested state passed into the Object.stop() command. |
| Num Stops Node | treenode | If this is a stop operation, the value of this node should be incremented by 1 within this function. If it is a resume operation, the value of this node should be decremented by 1 within this function. Also, you will get the value of this node to know how many stop commands have been called on this object. When the object is stopped for the first time (the value of the node goes from 0 to 1), you should execute logic specifying how to stop this object. When the object is resumed for the final time (the value of the node goes from 1 to 0), you should execute logic specifying how to resume this object. |
| Last Stop Time Node | treenode | When this object is stopped for the first time, this node should be set to the current time, so you can know the amount of time the object was stopped when it is finally resumed. |
| State Before Stop Node | treenode | When this object is stopped for the first time, this node should be set to the object's current state, so that you can know which state to go back to when the object is resumed. |
| State Profile | int | The profile number of the stopped state. The default state profile is 0. |

 

If you are implementing your own stop/resume logic here and do not want the default logic to be executed, you must return 1 from this function. If the object gets a return value of 0 from this function, it will assume that nothing was done, and will execute its own default logic.

 

 

 

## States

 

For statistical purposes, the BasicFR will be in one of the following states at various points during a simulation run. The current state can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Idle

 

The object is empty.

 

### Setup

 

The object is in its modeller-defined setup time.

 

### Processing

 

The object is in its modeller-defined process time.

 

### Blocked

 

The object has released its flow item(s), but downstream objects are not ready to receive them yet.

 

### Waiting for Operator

 

The object is waiting for an operator to arrive, either to repair a breakdown, or to operate on a batch.

 

### Waiting for Transport

 

The object has released a flow item, and a downstream object is ready to receive it, but a transport object has not picked it up yet.

 

 

 

## Statistics

 

The BasicFR uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The BasicFR object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Output
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The BasicFR object shares several properties with all fixed resources:

 

- Fixed Resource Properties
