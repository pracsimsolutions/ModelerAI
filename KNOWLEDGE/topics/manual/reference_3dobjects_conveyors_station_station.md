---
id: reference_3dobjects_conveyors_station_station
name: "Station"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","Station","Station"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/Station/Station.html"
---

# Station

## Overview and Key Concepts

 

A station is an object you can use to add processing points into the conveyor system. The station works very much like a standard Processor object, except that it is placed as a point in the conveyor system, instead of as an object outside of the conveyor system.

 

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The station has the events of a Decision Point and the following additional event:

 <!-- List events in alphabetical order with spaces in the event name --> 

### On Process Finish

 

On Process Finish fires immediately after the station finished processing an item.

 

 

 

## States

 

>For statistical purposes, the station will be in one of the following states at various points during a simulation run. The current state can be viewed by clicking on the object and then viewing the Statistics pane in Properties.

 

### Idle

 

The station is not processing an item.

 

### Processing

 

The station is processing an item.

 

### Waiting for Operator

 

The station is waiting for an operator to come to process the item.

 

 

 

## Statistics

 

The station tracks the following statistics. These can be viewed by clicking on a station and then viewing the Statistics pane in Properties.

 

### Content

 

The number of items being processed by the station. This will always be either 0 or 1.

 

### Input

 

The total number of items that have started being processed by the station. Note that this may be different than the total number of items that have conveyed over the station, depending on whether the **Process Condition** always processes items.

 

### Output

 

The total number of items that have finished being processed by the station. Like input, this may be different than the total number of items that have conveyed over the station.

 

### Staytime

 

The time from when an item arrives at the station to the time it is finished being processed. This is only recorded for items that are actually processed at the station.

 

 

 

## Properties Panels

 

The Station uses the following properties panels:

 

- Statistics
- Labels
- Decision Point
- Station
- Ports
- Triggers

 

 

 

## Properties

 

The Station object uses the following properties:

 

| Property | Type |
| --- | --- |
| NumOperators | Number |
| OperatorPreempt | Options |
| OperatorPriority | Number |
| OperatorRef | Number |
| ProcessCondition | Number |
| ProcessTime | Unit Code |
| UseOperators | Boolean |
