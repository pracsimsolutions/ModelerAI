---
id: reference_tools_mtbfmttr_mtbfmttr
name: "MTBF/MTTR"
kind: manual
breadcrumb: ["Reference","Tools","MtbfMttr","MtbfMttr"]
tags: ["manual","reference","tools","mtbfmttr"]
source: "manual/Reference/Tools/MtbfMttr/MtbfMttr.html"
---

# MTBF/MTTR

## Overview and Key Concepts

 

 

MTBF/MTTR objects are used to set random breakdown and recovery times for groups of objects in the model. Each MTBF/MTTR object can have any number of object members and each object can be controlled by more than one MTBF/MTTR object. The MTBF/MTTR object allows you to also specify what state the objects will go into when they go down and what behaviour they should perform. A model may contain any number of MTBF MTTR objects.

 

Though similar to the Time Table, the MTBF/MTTR object uses picklists to determine dynamically when the connected members will break down and how long they will be broken down for. You can also specify more specific information about the breakdowns. This includes specifying if all connected members will breakdown together, or if the breakdown times will be individually calculated for each object (firing the First Failure Time, Up Time, and Down Time picklists once per object). If you only want the connected members to count specific states towards their Up Time, for example a Processor when it is in the *processing* state, this can be specified as well. This means when the Processor is *idle*, elapsing time won't count towards the Processor's Up Time until it enters the *processing* state.

 

MTBF/MTTR objects are accessed from the Toolbox.

 

 

 

## General Properties

 

The MTBF/MTTR tool has the following general properties:

 

| Icon | Description |
| --- | --- |
|  | Adds the MTBF/MTTR to a User Library as either a Draggable Icon or an Auto-Install Component. |

 

### Name

 

The name of the MTBF/MTTR. The combobox has a list of all MTBF/MTTR objects in the model, allowing you to quickly jump to different MTBF/MTTR objects.

 

### Enabled

 

Specifies whether the MTBF/MTTR should execute it's down times for all of the members of the MTBF/MTTR. The Experimenter allows you to enable and disable MTBF/MTTRs for different scenarios.

 

 

 

## The Members Tab

 

The Members tab has the following properties:

 

| Icon | Description |
| --- | --- |
|  | This will open an object selection GUI where you can select multiple objects in the model. |
|  | Removes the selected member(s) from the list. |
|  | Reorders members up or down in the list. |
|  | Click to enter "Sample" mode, then click on any object in the model to add it as a member. |
|  | Click to find the selected member in the 3D view. |
|  | Click to select the selected member in the 3D view. |

 

 

 

## The Functions Tab

 

The Functions tab has the following properties:

 

 

The following picklists can be fired individually for each object, or for all the objects together depending on the checked state of the Break down members individually property from the Breakdowns Page.

 

### First Failure Time

 

This picklist returns the time of the first failure. Returning a negative number will cause the first failure to be ignored.

 

### Down Time

 

This picklist returns the Mean Time To Repair for the objects controlled by this MTBF MTTR object. This function determines how long they will stay in a broken-down state before resuming normal operations again. All of the controlled objects will go back to their original states at the same time.

 

### Up Time

 

This function determines how long the objects controlled by this MTBF MTTR object will run before they go into a broken-down state. The Up Time is specifically defined as the span between the time that the object resumes from its last down period and the time that it starts its next down period.

 

### Down Behavior

 

If a Down Behavior is selected in this drop down, the Down Function and Resume Functions described next will not be fired and instead the Down and Resume Functions of the Down Behavior will be fired. The properties of the selected Down Behavior can be edited by clicking the ... button. If Custom is selected from the drop down, the Down and Resume functions immediately below the drop down will be fired.

 

### Down Function

 

This picklist is executed when the objects in the member list go down. It is executed once for each object in the member list. Here is where you specify what to do to stop the object.

 

### Resume Function

 

This picklist is executed when the objects in the member list resume their operation. It is executed once for each object in the member list. Here is where you specify what to do to resume the object.

 

### OnBreakDown

 

This picklist is fired immediately after the Down Function, but it is only executed once, instead of once for each object.

 

### OnRepair

 

This picklist is fired immediately after the Resume Function, but it is only executed once, instead of once for each object.

 

 

 

## The Breakdowns Tab

 

The Breadowns tab has the following properties:

 

 

### Down State

 

This specifies the state that the object will go into when it goes down.

 

### Break down members individually

 

If this box is checked, the MTBF/MTTR object will create a separate thread of down and resume events for each member object. If it is not checked, all member objects will go down and resume at the same time.

 

### Apply MTBF to a set of states

 

This box only applies if the MTBF/MTTR breaks down members individually. If it is checked, then the MTBF time will only be applied to a subset of the object's state. For example, if machine break down data only applies for when the machine is actively processing, then you would use this field. If checked, you will add a set of states to the list on the right from the list of possible states on the left.

 

 

 

## Events

 

### First Failure Time

 

This event is fired at the start of the model run and determines the first time the object will go down.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Involved | Object | The object that will go down |

 

### Up Time

 

This event is fired after an object is repaired and determines how long before the object breaks down again.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Involved | Object | The object being repaired |

 

### Down Time

 

This event is fired after an object goes down and determines how long before the object is repaired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Involved | Object | The object going down |

 

### Down Function

 

This event is fired once for each member when it breaks down.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Down Object | Object | The object going down |
| State | int | The state the object will go into |
| Down Time | double | The duration of the down time |
| Member Coupling | treenode | The member node of the MTBF that is coupled with the down object. |

 

### Resume Function

 

This event is fired once for each member when it is repaired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Down Object | Object | The object that was down and is now resuming |
| State | int | The state the object was in |

 

### On Break Down

 

This event is fired once after the Down Function has fired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Involved | Object | The object breaking down |

 

### On Repair

 

This event is fired once after the Resume Function has fired.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Involved | Object | The object being repaired |
