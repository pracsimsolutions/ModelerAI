---
id: modellogic_processflowbasics_linkingmodels_linkingmodels
name: "Linking Process Flows to 3D Models"
kind: manual
breadcrumb: ["ModelLogic","ProcessFlowBasics","LinkingModels","LinkingModels"]
tags: ["manual","modellogic","processflowbasics","linkingmodels"]
source: "manual/ModelLogic/ProcessFlowBasics/LinkingModels/LinkingModels.html"
---

# Linking Process Flows to 3D Models

## Introduction to Linking Process Flows to 3D Models

 

Process flows can run independently from a simulation model if needed. But process flows are probably most useful when they are linked to a 3D model. Process flows can help you build complex logic into your model, replacing the need for complicated custom FlexScript code. Process flows will also make it easier to troubleshoot your model logic if needed. With that in mind, this topic covers how to integrate process flows with a standard simulation model.

 

The main way to connect a process flow to a 3D model is through event-listening. (See Key Concepts About Events if you are unfamiliar with what the term *event* means at this point.) Event-listening process flow activities can be set to listen to specific events in the simulation model and then create or release a token to the next downstream activity when that event occurs.

  

#### Linking to Sub Flows

 

Although event-listening is the most common way to link a process flow, you should be aware that some properties on fixed resources and task executers have picklist menu options that can initiate a sub flow. See Sub Process Flows for more information.

  

Another method for connecting a process flow to a 3D model is to use the Object activities in the Process Flow Library. For example, the Create Object activity can act similar to a source and create a flow item and place it in the 3D model. The Destroy Object activity can act like a sink and and remove flow items from a model. And the Move Object activity can instantly move flow items from one object to another in the model.

 

Since it is fairly intuitive to use the Object activities, they won't be discussed in detail in this topic. The rest of this topic will focus on how to connect process flows to the 3D model using event-listening activities.

 

 

 

## Event-Listening Activities

 

The two process flow activities that can listen for events are:

 

- **Event-Triggered Source** - This activity will listen for a specific event to occur in the simulation model. When that event occurs, it will create one or more new tokens and send the tokens to the next activity in the process flow. You should use an event-triggered source if you want the triggering event to initiate the process flow.
- **Wait for Event** - After this activity receives a token, it will hold the token while it listens for a specific event to occur in the simulation model (or in the process flow). When that event occurs, it will release the token to the next activity in the process flow. If needed, you can also make the Wait for Event act like a Decide activity. In other words, the Wait for Event activity could possibly send the token to different activities next based on certain conditions in the simulation model.

 

 

 

## Requirements for Event-Listening

 

When you are linking one of the event-listening activities to a 3D model, there are two fundamental questions that need to be answered:

 

1. Which *object* will the activity listen to?
1. Which *event* on that object will trigger the creation and/or release of a token?

 

In other words, you need both an object and event to set up an event-listening activity. These two requirements correlate with the *Object* and *Event* properties on the Wait for Event and Event-Triggered Source activities:

 

 

### Object

 

You have a couple of different options when deciding which object to link to:

 

- **Static Object** - You can link to a specific static object in your simulation model such as a 3D object or another activity in a process flow.
- **Dynamic Object** - You can set the object to `current` on any type of process flow except general process flows. The event-listening activity will listen to the object that is attached to each instance of that process flow. (See Process Flow Instances for more information.)
- **Groups of Objects** - You can link to a group of objects and the activity will listen for any time the event fires on any member of that group.
- **Flow items or tokens** - You can link dynamically to flow items or tokens. For example, you could listen to flow items exiting a fixed resource or tokens leaving an activity.
- **Modeling Tools** - Many of the tools found in the toolbox have events associated with them that you can listen to. This is especially useful for objects like the Time Table that will allow you to listen for down and up times.

 

### Event

 

In general, there are two different types of events:

 

- **Standard Events** - The normal simulation events that occur during a simulation run, such as an item entering a fixed resource or a token entering an activity in a process flow. Each event has a trigger associated with it that will fire when that event occurs. See Key Concepts About Events for a more thorough explanation of simulation events and triggers.
- **Statistical Events** - Statistical events occur when a statistical value on a object changes or when the object enters into a specific state. For example, you could listen for when the number of items inside a queue changes, or when a processor changes to an idle state, etc.

 

The event-listening activity will create and/or release a token whenever the activity you select occurs in the simulation model.

  

#### Sometimes the Object and Event Might Be the Same

 

In some cases, the event will be the same as the object. For example, you may connect directly to a node in the tree such as a user command. In this case, the *Object* property will be a reference to the node, while the *Event* property will say *Same as Object*.

  

 

 

## Setting Up an Event-Listening Activity

 

The method you will use to set up an event-listening activity will depend on the type of object you want to listen to. The following sections explain how to listen to different types of objects.

  

#### Splitting the Panes

 

When setting up event-listening, it's often easier if you split the center pane so that it is split between the 3D model and the process flow tab. See Arranging Windows and Tabs for more information.

  

### Listening to a Static Object

 

Before you can set up event-listening for a static object, make sure the object you want to listen has been added to your 3D model or process flow.

 

To set up event-listening for a static object:

 

1. With the process flow tab active, add a **Wait for Event** or **Event-Triggered Source** to the process flow.
1. Click the activity's icon to open its properties, as shown in the following image:
1. Next to the **Object** box, click the **Sampler** button to enter sampling mode. Click the object you want to listen to. This will open a menu that will list all of the possible events available for that object. Select the appropriate event from the menu.
1. The object and event you selected will now be listed in both the **Object** and **Event** boxes in the activity's properties. Based on the type of event you selected, some additional properties might now be available in the activity's properties.

  

#### Shortcut for Adding Events

 

There is a faster way to add an event to an Event-Triggered Source or Wait for Event activity. When you first add one of those activities to your process flow, you'll notice that an Exclamation mark next to the activity. If you click on this icon, you will enter sampling mode. In sampling mode, you can click object that you want the activity to listen to. This shortcut is the same as clicking the Sampler button in steps 5 and 6.

  

### Listening to a Dynamic Object

 

Be aware that you cannot listen to a dynamic object on a general process flow. But the other three types of process flows have this ability.

 

Before setting up event-listening for a dynamic object, you might want to add at least one of the type of objects to which you will listen to the 3D model or process flow. Even though you won't link to this object directly, you'll need this object so that you can sample its events.

 

To set up event-listening for a dynamic object:

 

1. With the process flow tab active, add a **Wait for Event** or **Event-Triggered Source** to the process flow.
1. Click the activity's icon to open its properties, as shown in the following image:
1. Check to ensure that the **Object** box says `current`, which is the default value for this property. If it says something else, click the arrow next to the **Object** box to open a menu. Select **current (Instance Object)** from the menu.
1. Next to the **Event** box, click the **Sampler** button to enter sampling mode. Click the object you want to listen to. This will open a menu that will list all of the possible events available for that object. Select the appropriate event from the menu.
1. The event you selected will now be listed in the **Event** box in the activity's properties. Based on the type of event you selected, some additional properties might now be available in the activity's properties.

 

### Listening to a Group

 

Before setting up event-listening for a group, you need to first create the group. Make sure the group has at least one member. Even though you won't link to this object directly, you'll need this object so that you can sample its events.

 

To set up event-listening for a group:

 

1. With the process flow tab active, add a **Wait for Event** or **Event-Triggered Source** to the process flow.
1. Click the activity's icon to open its properties, as shown in the following image:
1. Click the arrow next to the **Object** box to open a menu. Point to **Groups**, then select the name of the group you want to listen to.
1. Next to the **Event** box, click the **Sampler** button to enter sampling mode. Click the object you want to listen to. This will open a menu that will list all of the possible events available for that object. Select the appropriate event from the menu.
1. The event you selected will now be listed in the **Event** box in the activity's properties. Based on the type of event you selected, some additional properties might now be available in the activity's properties.

 

### Listening to a Flow Items

 

To listen to flow items, you'll need to create a dynamic reference to them on the event-listener. Before setting up event-listening for a flow item, you need to add a source to your 3D model. Before setting up event-listening for a token, you need to make sure the activity to which you want to listen has been added to the process flow.

 

To set up event-listening for a flow item:

 

1. With the process flow tab active, add a **Wait for Event** or **Event-Triggered Source** to the process flow.
1. Click the activity's icon to open its properties, as shown in the following image:
1. Next to the **Object** box, click the **Sampler** button to enter sampling mode. Click the **Source** in the 3D model. This will open a menu that will list all of the possible events available for that object. Select the **Source: On Creation** from the menu.

 

### Listening to Tokens

 

To listen to tokens, you'll need to create a dynamic reference to them on the event-listener. Before setting up event-listening for a token, you need to make sure the activity to which you want to listen has been added to the process flow.

 

To set up event-listening for a flow item or token:

 

1. With the process flow tab active, add a **Wait for Event** or **Event-Triggered Source** to the process flow.
1. Click the activity's icon to open its properties, as shown in the following image:
1. Next to the **Object** box, click the **Sampler** button to enter sampling mode. Click the activity in the process flow you want to listen. This will open a menu that will list all of the possible events available for tokens. Select the appropriate event from the menu.
1. The object and event you selected will now be listed in both the **Object** and **Event** boxes in the activity's properties. Based on the type of event you selected, some additional properties might now be available in the activity's properties.

 

### Listening to a Tool

 

You can use the sampler to hover over objects in the Toolbox and select the desired event, or you can hover over the picklists in their properties windows to select one of these events.

 

 

 

## Available Events

 

Flow items have the following events:

 

| Event | Description |
| --- | --- |
| On Entering | Fired when the flow item enters a fixed resource or is loaded by a task executer object |
| On Exiting | Fired when the flow item exits a fixed resource or is unloaded by a task executer object. |

 

Tokens have the following events:

 

| Event | Description |
| --- | --- |
| On Entering | Fired when the token enters an activity |
| On Exiting | Fired when the token exits an activity. |
| On Manual Release | Fired when the token is manually released from an activity using the releasetoken() command |
| On Asset Allocated | Fired when the token allocates a shared asset. This may be acquiring a resource, entering a zone being pushed onto a list or creating a backorder when pulling from a list. |
| On Asset Deallocated | Fired when the token deallocates a shared asset. This may be releasing a resource, exiting a zone being pulled from a list, or completing a backorder. |
| On Label Value Change | Fired when a label value changes. This is only available if a label is stored as a tracked variable. |

 

 <!-- 

 

## Label Assignment

 

 

 

 

## Change Rule

 

 

 -->
