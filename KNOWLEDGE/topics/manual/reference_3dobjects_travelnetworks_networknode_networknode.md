---
id: reference_3dobjects_travelnetworks_networknode_networknode
name: "Network Node"
kind: manual
breadcrumb: ["Reference","3DObjects","TravelNetworks","NetworkNode","NetworkNode"]
tags: ["manual","reference","3dobjects","travelnetworks"]
source: "manual/Reference/3DObjects/TravelNetworks/NetworkNode/NetworkNode.html"
---

# Network Node

## Overview and Key Concepts

 

Network nodes are used to define a network of paths that transporters and operators follow. The paths can be modified using spline points to add curvature to the path. By default, objects travelling on a network will follow the shortest path between their origin and destination.

 

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The network node has the following events. Each time a task executer travels on the network, it fires the OnArrival and OnContinue for each network node involved in the travel operation.

 <!-- List events in alphabetical order with spaces in the event name --> 

### On Arrival

 

On Arrival fires when a traveler arrives at a node.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | Object | The object traversing the network |
| To Edge | int | The rank of the edge the traveler will continue on |
| From Edge | int | The rank of the edge the traveler arrived on |

 

For the first node in a travel operation, the From Edge will usually be 0. If the traveler is configured to block space on the network, the From Edge will be whatever edge it came from previously.

 

If the On Arrival trigger returns a value greater than 0, then the traveler will reroute to the path returned. For example, if On Arrival returns 2, then the traveler will continue its travel to the second connection path out of the network node.

 

### On Continue

 

On Continue fires right before a traveler leaves a node.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | Object | The object traversing the network |
| To Edge | int | The rank of the edge the traveler will continue on |
| From Edge | int | The rank of the edge the traveler arrived on |

 

For the last node in a travel operation, the To Edge will be 0.

 

 

 

## States

 

The network node does not implement any states.

 

 

 

## Statistics

 

The network node does not track any statistics.

 

 

 

## Properties Panels

 

The Network Node object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Network Node
- Ports
- Triggers

 

 

 

## Properties

 

The Network Node object uses the following properties:

 

| Property | Type |
| --- | --- |
| MaxTravelers | Number |
| SideOffset | Unit |
