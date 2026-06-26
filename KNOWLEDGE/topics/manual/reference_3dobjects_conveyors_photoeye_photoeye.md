---
id: reference_3dobjects_conveyors_photoeye_photoeye
name: "Photo Eye"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","PhotoEye","PhotoEye"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/PhotoEye/PhotoEye.html"
---

# Photo Eye

## Overview and Key Concepts

 

A photo eye is a versatile object you can use to build logic into your conveyor system. Photo eyes can also be linked to a merge controller to notify it when a released slug has cleared designated points in the system.

 

 

A photo eye can be in one of four states while it is interacting with items on a conveyor: blocked, cleared, blocking and clearing. Triggers fire when the photo eye goes into the blocked and cleared states. The following image illustrates the various photo eye states and triggers while interacting with items on a conveyor:

 

You can assign a photo eye custom behaviors or actions that it can perform when it enters either a blocked or cleared state. You can also define how long a photo eye must be in the "blocking" or "clearing" state before it moves to the "blocked" or "cleared" state. The colors for the "blocking", "blocked", and "clearing" states are defined on the properties of the Conveyor System.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The photo eye has the following events:

 <!-- List events in alphabetical order with spaces in the event name --> 

### On Cover

 

On Cover fires when an item covers the photo eye.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Covering Item | Object | The item covering the photo eye |
| Conveyor | Conveyor | The conveyor the photo eye is on |

 

### On Block

 

On Block fires after an item covers the photo eye and the Block Time elapses before the photo eye is uncovered.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Blocking Item | Object | The item blocking the photo eye |
| Conveyor | Conveyor | The conveyor the photo eye is on |

 

### On Uncover

 

On Uncover fires when an item moves out of the photo eye leaving it uncovered.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Uncovering Item | Object | The item uncovering the photo eye |
| Conveyor | Conveyor | The conveyor the photo eye is on |

 

### On Clear

 

On Clear fires after an item moves out of the photo eye and the Clear Time elapses before the photo eye is covered again.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Clearing Item | Object | The item covering the photo eye |
| Conveyor | Conveyor | The conveyor the photo eye is on |

 

 

 

## States

 

The photo eye implements state using two different state profiles, namely the default state profile, and a custom "PhotoEye" state profile. The custom profile uses the following states:

 

### Clear

 

The photo eye is completely clear.

 

### Blocking

 

The photo eye was covered from a clear state, but its block time has not yet expired.

 

### Blocked

 

The photo eye is covered and its block time has expired.

 

### Clearing

 

The photo eye was uncovered from a blocked state, but its clear time has not yet expired.

 

The default state profile uses the following states. The current state can be viewed by clicking on the object and then viewing the Statistics pane in Properties.

 

### Idle

 

The idle state corresponds to the clear plus blocking states in the photo eye's custom state profile.

 

### Blocked

 

The blocked state corresponds to the blocked plus clearing states in the photo eye's custom state profile.

 

 

 

## Statistics

 

The photo eye tracks the following statistics. These can be viewed by clicking on the object and then viewing the Statistics pane in Properties.

 

### Content

 

The number of items that have covered the photo eye but have not yet cleared it. This is usually either 0 or 1, but if the photo eye has an offset angle, it may be greater than 1.

 

### Input

 

The total number of items that have covered the photo eye.

 

### Output

 

The total number of items that have cleared the photo eye.

 

 

 

## Properties Panels

 

The Photo Eye uses the following properties panels:

 

- Statistics
- Labels
- Photo Eye
- Ports
- Triggers

 

 

 

## Properties

 

The Photo Eye object uses the following properties:

 

| Property | Type |
| --- | --- |
| BlockTime | Unit |
| ClearTime | Unit |
| DensityThreshold | Number |
| MassFlowMode | Options |
| OffsetAngle | Number |
| Height | Unit |
| RequireGapToClear | Boolean |
| SpeedThreshold | Unit |
