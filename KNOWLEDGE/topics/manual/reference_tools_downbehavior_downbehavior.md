---
id: reference_tools_downbehavior_downbehavior
name: "Down Behavior"
kind: manual
breadcrumb: ["Reference","Tools","DownBehavior","DownBehavior"]
tags: ["manual","reference","tools","downbehavior"]
source: "manual/Reference/Tools/DownBehavior/DownBehavior.html"
---

# Down Behavior

## Overview and Key Concepts

 

 

Down Behaviors represent a specific kind of down event. They can be called by Time Tables and MtbfMttrs.

 

For example, you might want to add a down event representing what an operator should do when they take their lunch. You would add a new Down Behavior and configure its functions and labels so that the operators go to lunch. Then in the Time Tables the operators are members of, you would tell the lunch down event to use the new Down Behavior's functions instead of the Time Table's.

 

Down Behavior objects are accessed from the Toolbox.

 

 

 

## General Properties

 

The Down Behavior tool has the following general properties:

 

| Icon | Description |
| --- | --- |
|  | Adds the Down Behavior to a User Library as either a Draggable Icon or an Auto-Install Component. |

 

### Name

 

The name of the Down Behavior.

 

 

 

## The Functions Tab

 

The Functions tab has the following properties:

 

 

### Down Function

 

This picklist is executed when the objects in the member list go down. It is executed once for each object in the member list. This is where you specify what to do to stop the object.

 

### Resume Function

 

This picklist is executed when the objects in the member list resume their operation. It is executed once for each object in the member list. This is where you specify what to do to resume the object.

 

### On Down

 

This picklist is fired immediately after the Down Function has been fired for all objects, but it is only executed once, instead of once for each object in the member list.

 

### On Resume

 

This picklist is fired immediately after the Resume Function has been fired for all objects, but it is only executed once, instead of once for each object in the member list.

 

 

 

## The Labels Tab

 

The The Labels Tab is where you add labels to the Down Behavior. This allows you to further customize the behavior of the down event.

 

 

 

## Events

 

### Down Function

 

This event is fired once for each member of a Time Table or MTBF when it breaks down.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Time Table | Object | The Time Table or MTBF/MTTR that triggered the down event |
| Down Object | Object | The object going down |
| State | int | The state the object will go into |
| Duration | double | The duration of the down time |
| State Profile | int | The state profile on the object that will be updated |
| Table Row | int | The row corresponding to this down time in the table (Time Table only) |
| Member Coupling | treenode | The member node of the MTBF that is coupled with the down object (MTBF only) |

 

### Resume Function

 

This event is fired once for each member of a Time Table or MTBF when it is repaired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Time Table | Object | The Time Table or MTBF/MTTR that triggered the down event |
| Down Object | Object | The object that was down and is now resuming |
| State | int | The state the object was in |
| State Profile | int | The state profile on the object that will be updated |
| Table Row | int | The row corresponding to this down time in the table (Time Table only) |

 

### On Down

 

This event is fired after the Down Function has fired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Time Table | Object | The Time Table or MTBF/MTTR that triggered the down event |
| Table Row | int | The row corresponding to this down time in the table (Time Table only) |
| Involved | Object | The object going down |

 

### On Resume

 

This event is fired after the Resume Function has fired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Time Table | Object | The Time Table or MTBF/MTTR that triggered the down event |
| Table Row | int | The row corresponding to this down time in the table (Time Table only) |
| Involved | Object | The object being resumed |
