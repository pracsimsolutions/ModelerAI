---
id: reference_3dobjects_travelnetworks_trafficcontrol_trafficcontrol
name: "Traffic Control"
kind: manual
breadcrumb: ["Reference","3DObjects","TravelNetworks","TrafficControl","TrafficControl"]
tags: ["manual","reference","3dobjects","travelnetworks"]
source: "manual/Reference/3DObjects/TravelNetworks/TrafficControl/TrafficControl.html"
---

# Traffic Control

## Overview and Key Concepts

 

The traffic control object is used to control traffic in a given area of a travel network. You build a traffic controlled area by connecting network nodes to the traffic control object. These NetworkNodes then become members of the traffic controlled area. Any path between two nodes that are both members of the same traffic control object is designated as a traffic controlled path, and travelers are only allowed onto that path if given permission by the traffic control object. The traffic control object can be in a mutual exclusion mode, where it only lets a certain number of travelers into the area at any given time, or it can use un-timed traffic modes to only allow travelers onto certain path sections at once.

 

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The traffic control object has the following events:

 <!-- List events in alphabetical order with spaces in the event name --> 

### On Entry

 

On Entry occurs when a traveller enters the area controlled by this traffic control.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | treenode | The traveller (or task sequence) entering the control area |
| Network Node | treenode | The traveller's node, an entry to the control area |
| Next Output Port | int | The output port the traveller will use next |

 

### On Entry Request

 

On Entry Request occurs when a traveller attempts to enter an area controlled by the traffic control.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | treenode | The traveller (or task sequence) requesting entry the control area |
| Network Node | treenode | The traveller's node, an entry to the control area |
| Next Output Port | int | The output port the traveller will use next |

 

### OnExit

 

On Exit occurs when a traveller leaves the control area.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | treenode | The traveller (or task sequence) exiting the control area |

 

 

 

## States

 

The traffic control object does not implement states.

 

 

 

## Statistics

 

The traffic control object does not track any statistics.

 

 

 

## Properties Panels

 

The Traffic Control object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Traffic Control
- Ports
- Triggers

 

 

 

## Properties

 

The Traffic Control object uses the following properties:

 

- CenterObjects
- InObjects
- OutObjects
