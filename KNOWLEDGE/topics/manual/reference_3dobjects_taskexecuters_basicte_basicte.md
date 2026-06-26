---
id: reference_3dobjects_taskexecuters_basicte_basicte
name: "Basic TE"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","BasicTE","BasicTE"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/BasicTE/BasicTE.html"
---

# Basic TE

## Overview and Key Concepts

 

The Basic TE object is a task executer that is meant for developers to create user libraries with. It passes almost all inheritable task executer logic to picklist functions, so that user library developers can specify virtually all functionality for the task executer.

 

 

The Basic TE is a task executer. It allows you to specify logic for its offset travel functionality, as well as advanced functionality like stop object/resume object, pick/place offset, and other advanced functions.

 

 

 

## Events

 

The Basic TE uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

The Basic TE has the following additional events:

 

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

 

### On Begin Offset

 

The begin offset event is fired at the beginning of an offset travel operation. It will execute the On Begin Offset trigger. The event uses an x, y and z offset location. The object should figure out how it will travel the specified offset.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| X Location | double | The requested x offset. This is an offset distance from the x center of the object. |
| Y Location | double | The requested y offset. This is an offset distance from the y center of the object. |
| Z Location | double | The requested z offset. This is an offset distance from the z base of the object. |
| Item | Object | If the offset operation has an involved item, this is a reference to the item. |
| End Speed | double | The requested end speed for the offset travel operation. |
| Max Speed | double | The value of this object's maximum speed variable. |
| Acceleration | double | The value of this object's acceleration variable. |
| Deceleration | double | The value of this object's deceleration variable. |
| Last Update Speed | double | The end speed of the object's last travel operation. |
| Rotate While Traveling | int | This is a 1 or 0, and specifies whether the user wants the object to rotate while traveling. This value is defined in the object's properties. |

 

The return value of the on begin offset is the amount of time it will take to make the travel operation.

 

### On Finish Offset

 

The on finish offset event is fired when the object has finished its offset operation. This should update the location of the object to its final offset location. This event will fire the object's on finish offset trigger.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Offsetting Now | double | If the object is currently doing an offset operation, this will be 1, otherwise 0. The offsetting now value is automatically set to 1 when On Begin Offset fires, and then set back to 0 when On Finish Offset fires. |
| Offset Total Time | double | The total time this object returned from its On Begin Offset trigger. |
| Max Speed | double | The value of this object's maximum speed variable. |
| Acceleration | double | The value of this object's acceleration variable. |
| Deceleration | double | The value of this object's deceleration variable. |
| Last Update Speed | double | The end speed of the object's last travel operation. |
| Rotate While Traveling | double | This is a 1 or 0, and specifies whether the user wants the object to rotate while traveling. This value is defined in the object's properties. |
| Load/Unload Time | double | If the offset operation is a load or unload operation, then this value is the load/unload time for the operation. |

  

#### Non-Zero Load/Unload Time

 

If there is a non-zero load/unload time, then this time will be executed before the On Finish Offset event is fired. This means that in your update offset function, you may need to query whether you're in the offset part of the operation, or in the load/unload part of the operation. Usually this will not matter though, because the travel operations will finish, and during the remaining load/unload time, the update function will automatically set the object's location to the final destination location. You will really only need to use this if you are going to do some animation/movement during the load/unload time, as the ASRSvehicle and Elevator objects do.

  

### On State Change

 

This event is fired when the object's state is changed either through the `Object.setState()` command or through the process of executing task sequences.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| State | int | The new state the object is changing to. |
| State Profile | int | The profile number of the new state. The default state profile is 0. |

 

If the function returns a 0, the object will proceed to change its state. If the function returns -1, the object will do nothing, leaving it in its previous state. If the function returns a positive number, the state will be changed to this new state number.

 

### On Update Offset

 

The on update offset event is fired before the view is refreshed and the object is drawn. This is where the object updates its location based on its current offset operation.

 

This event shares the same parameters as the On Finish Offset event.

 

### Pick Place Offset

 

The pick place offset event is fired at the same time as the task executer standard events, pick offset and place offset. The difference is in what parameters are available. For place offset, this event is fired when another transport object is attempting to place, or unload, a flow item into this object. For pick offset this event is fired when a transport is about to pick, or load, an item from this object.

 

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

  

#### Loading and Unloading Items

 

This function does not fire when this object is attempting to load/unload to or from another object, but rather when another object is attempting to load/unload to or from this object.

  

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

 

The Basic TE uses the standard task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The Basic TE uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Basic TE object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Basic TE object shares several properties with all task executers:

 

- Task Executer Properties
