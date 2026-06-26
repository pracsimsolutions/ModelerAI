---
id: reference_3dobjects_fixedresources_sink_sink
name: "Sink"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","Sink","Sink"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/Sink/Sink.html"
---

# Sink

## Overview and Key Concepts

 

The sink is used to destroy flow items that are finished in the model. Once a flow item has traveled into a sink, it cannot be recovered. Any data collection involving flow items that are about to leave the model should be done either before the flow item enters the sink or in the sink's OnEntry trigger.

 

 

The sink is a fixed resource. Since it destroys all flow items it receives, the sink does not have any output logic.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The sink uses some of the same standard events that are common to all fixed resources which include:

 

- On Entry
- On Message
- Place Offset
- Pull Strategy
- Pull Requirement
- On Stop
- On Resume
- On State Change
- On State Value Change
- On Input Change
- On Draw

 

See Fixed Resources - Events for an explanation of these events.

 

 

 

## States

 

The sink is always in the collecting state (unless the object is stopped and the state is set). Refer instead to its input statistics.

 

 

 

## Statistics

 

The sink tracks the following statistics. These can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Input

 

The total number of items that have entered the sink.

 

 

 

## Properties Panel

 

The Sink object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The Sink object shares several properties with all fixed resources:

 

- Fixed Resource Properties
