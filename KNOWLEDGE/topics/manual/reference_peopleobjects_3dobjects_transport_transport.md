---
id: reference_peopleobjects_3dobjects_transport_transport
name: "Transport"
kind: manual
breadcrumb: ["Reference","PeopleObjects","3DObjects","Transport","Transport"]
tags: ["manual","reference","peopleobjects","3dobjects"]
source: "manual/Reference/PeopleObjects/3DObjects/Transport/Transport.html"
---

# Transport

## Overview and Key Concepts

 

The Transport object is a specialized Task Executer designed to work with the Acquire and Release Transport activities. It is moved around the model by staff and used to transport person flowitems. It also defines the behavior of people who enter and exit it.

 

 

 

 

## Events

 

The transport uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

### On Acquire

 

The on acquire event fires when the transport is acquired by an Acquire Transport activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Pulling Token | Token | The token in the Acquire Transport activity. |
| Puller | Object | The object acquiring the transport. |

 

### On Release

 

The on release event fires when the transport is released by a Release Transport activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Releasing Token | Token | The token in the Release Transport activity. |

 

This object also uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The transport uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Transport object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Enter/Exit Instructions
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers
