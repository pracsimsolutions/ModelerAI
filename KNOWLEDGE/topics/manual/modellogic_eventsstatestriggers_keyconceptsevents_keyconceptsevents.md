---
id: modellogic_eventsstatestriggers_keyconceptsevents_keyconceptsevents
name: "Key Concepts About Events"
kind: manual
breadcrumb: ["ModelLogic","EventsStatesTriggers","KeyConceptsEvents","KeyConceptsEvents"]
tags: ["manual","modellogic","eventsstatestriggers","keyconceptsevents"]
source: "manual/ModelLogic/EventsStatesTriggers/KeyConceptsEvents/KeyConceptsEvents.html"
---

# Key Concepts About Events

## Introduction to Events, States, and Triggers

 

In order to control the logic of your simulation model, you'll need a deeper understanding of events, states, and triggers. These three concepts are the foundation to simulating the logic of any business system.

 

 

 

## What Are Simulation Events?

 

Simulation events (also sometimes referred to as *standard events*) are the events that occur while a 3D object is interacting with flow items during a simulation run. Events can also refer to the events that occur when a process flow activity interacts with a token.

 

FlexSim is a *discrete event simulation* software program, which means it represents the way a business system works as a series of distinct events that occur over time. Some typical examples of events that occur in real-life business systems are:

 

- The arrival of a customer or a new order
- The arrival of a shipment of materials
- The transportation or conveyance of a product from one station to another
- Processing a product at a machine or station
- The breakdown of a machine

 

FlexSim simulates the logic of these real-life events using simulation events on 3D objects. You could think of each simulation event as a chunk of pre-programmed logic that tells the 3D object how to interact with flow items. You can modify the logic and behavior of the 3D objects when these events occur to make them more similar to the business system you are modeling.

 

Simulation events will occur in the same sequence every time the 3D object interacts with a flow item. For example, the processor 3D object has three basic phases:

 

 

The processor basically gets a flow item, processes the item, then sends the item to the next downstream object.

 

During each of these three phases, several different simulation events can occur, some of which may not be visible in the 3D model during a simulation run:

 

 

During each of these events, you can set the processor to use logic that will make it behave more like the business system you are trying to simulate. For example, you can implement port logic or list logic during the Get Item or Send Item phase, as explained in the chapter about Connecting 3D Object Flows.

 

You can also use logic during the phase in which the processor processes items. For example, you could set the logic that determines how long the setup and/or processing times will take. The setup and processing times can be a set amount of time, a random amount of time based on a statistical distribution, or it could depend on certain dynamic conditions (such as the item's product type).

 

The specific sequence of events will depend on what class of object it is (such as whether it is a fixed resource or task executer). Beyond that, each type of 3D object will have a few additional events that are unique to its type. For a detailed technical explanation of each event, see Fixed Resource Concepts and Task Executer Concepts. See the reference pages for individual 3D objects if you'd like an explanation of the events that are unique to each 3D object.

 

 

 

## What Are States?

 

When events occur in a real-life business system, they sometimes create a change of state in that system. For example, when a machine is processing an item, it is in a *processing* state. When it is not processing an item, it is in an *idle* state. It could also be in a *down* state while it is broken or being repaired.

 

Similarly, events in a simulation model can sometimes cause a state change in the 3D objects for that system. For example, an operator could be in a *traveling* state while they are moving from one object to another. They could also be in a *utilized* state, meaning they are working on a particular task.

 

States are usually important for statistical and data-gathering purposes. For example, you might want to analyze the ratios of time your operators are in a utilized state vs. traveling vs. idle to determine how efficient your business system is. However, you could also cause certain events to happen when an object changes states or enters into a specific state. For example, when a processor is down, it could possible send a message that starts a sequence of repair tasks and events.

 

Your simulation model can listen for state changes or other statistical changes on 3D objects. For example, your model could track when a queue is empty, when it reaches its maximum content, when its content changes, etc.

 

 

 

## What Are Triggers?

 

Many of the events and statistical changes in the simulation model are associated with a specific trigger. A trigger is logic that is implemented whenever that event occurs in the model. You can assign logic to a trigger, which means that when that trigger fires, it will cause a chain reaction of other behaviors or events.

 

Referring back to the flowchart of events on a processor that was referenced earlier, certain events are associated with specific triggers. For example, at the end of a processor's Get Item phase, the OnEntry trigger fires:

 

 

During the Process Item phase, the OnSetupFinish and the OnProcessFinish triggers will both fire at different points:

 

 

And after the Send Item phase, the OnExit trigger fires:

 

 

You could add logic to any of these triggers. For example, you could add logic that changes the color or shape frame of a flow item when the OnProcessFinish trigger fires to indicate that something about the flow item has been changed as a result of going through the processor. Or you could add logic that sends a message to another object in the model whenever the OnExit trigger fires. Or you could initiate a chain reaction of logic in a Process Flow when a particular event fires.

 

FlexSim comes with a variety of commonly-used, pre-programmed logic that you can add to these triggers. This standard logic is sometimes referred to as *picklists*. You could create your own custom logic on these triggers using the Process Flow tool or FlexScript.

 

 

 

## Tool and Model Events

 

Some of the objects used by various modeling tools in FlexSim have events associated with them. The simulation model itself also has a few events associated with it.

 

| Tool | Events |
| --- | --- |
| Process Flow | Each process flow activity has its own set of events that tells the process flow activity how to interact with tokens. You can listen for triggers to fire when certain events or statistical changes occur on a process flow activity, such as: - OnEntry - OnExit - OnContentChange |
| Time Tables | If you use time tables to schedule down time for objects, you can listen to the On Down and On resume events on the time table. |
| MTBF/MTTR | Listen to any of the MTBF/MTTR functions found on the Functions page. |
| Global Lists | Global Lists have many different Standard Events and Value Change events. |
| User Commands | User commands execute code stored on a node. Any node that is evaluated using nodefunction() like user commands can be listened to. |
| Model Triggers | Similar to user commands, model triggers like On Model Open can can also be listened to. |

 

 

 

## Adding Logic to an Event, State, or Trigger

 

As was mentioned earlier, FlexSim comes with standard logic that you can add to a 3D object. You can add this kind of content using the 3D object's properties. For example, you would set a fixed resource's port or list logic under the Input or Output sections. (See Connecting 3D Object Flows for more information.) Or you would add logic to a trigger on the 3D object's Triggers tab. You would set a processor's setup and processing logic on the Processor tab.

 

If you wanted to create custom logic, you would likely create a process flow that would listen for certain events in the simulation model.<!-- See <_a href="../../ProcessFlowBasics/LinkingModels/LinkingModels.html">Linking Process Flows to 3D Models for more information.-->

 

If you are familiar with FlexScript, you could also add custom coded logic to triggers or properties in an object's properties.
