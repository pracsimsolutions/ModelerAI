---
id: reference_peopleobjects_3dobjects_location_location
name: "Location"
kind: manual
breadcrumb: ["Reference","PeopleObjects","3DObjects","Location","Location"]
tags: ["manual","reference","peopleobjects","3dobjects"]
source: "manual/Reference/PeopleObjects/3DObjects/Location/Location.html"
---

# Location

## Overview and Key Concepts

 

The Location object represents objects, such as chairs and beds, that person flowitems and staff occupy and interact with. It is designed to work with the Acquire and Release Location activities. It also defines the behavior of people who enter and exit them.

 

 

 

 

## Events

 

### On Acquire

 

The on acquire event fires when the location is acquired by an Acquire Location activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Pulling Token | Token | The token in the Acquire Location activity. |
| Puller | Object | The object acquiring the location. |

 

### On Release

 

The on release event fires when the location is released by a Release Location activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Releasing Token | Token | The token in the Release Location activity. |

 

### On Entry

 

The on entry event will fire the object's on entry trigger. This event fires as soon as an item enters the object, before any other logic is executed. .

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Entering Item | Object | The flow item that entered the object. |
| Input Port | int | Unused |

 

### On Exit

 

The on exit event will fire the object's on exit trigger. This event fires as an item is leaving the object, before it has actually left the object. This means if you check the content of the object in the on exit trigger, the content will include the exiting item.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Exiting Item | Object | The flow item that is exiting the object. |
| Output Port | int | Unused |

 

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

 

 

 

## States

 

### Idle

 

The object is empty and not performing any tasks.

 

### Maintenance

 

The object is having maintenance performed on it.

 

### Occupied

 

The object is occupied by a person flowitem.

 

### Waiting for Maintenance

 

The object is waiting for maintenance to be performed on it.

 

 

 

## Statistics

 

The location uses the same statistics as a fixed resource. See Fixed Resource Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Location object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Location
- Enter/Exit Instructions
- Ports
- Triggers
