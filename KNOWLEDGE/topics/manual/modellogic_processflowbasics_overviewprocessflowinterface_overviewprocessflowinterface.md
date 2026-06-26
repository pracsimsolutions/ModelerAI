---
id: modellogic_processflowbasics_overviewprocessflowinterface_overviewprocessflowinterface
name: "Overview of the Process Flow Interface"
kind: manual
breadcrumb: ["ModelLogic","ProcessFlowBasics","OverviewProcessFlowInterface","OverviewProcessFlowInterface"]
tags: ["manual","modellogic","processflowbasics","overviewprocessflowinterface"]
source: "manual/ModelLogic/ProcessFlowBasics/OverviewProcessFlowInterface/OverviewProcessFlowInterface.html"
---

# Overview of the Process Flow Interface

## Introduction

 

This topic will provide a high-level overview of the most important elements of the Process Flow tool and will explain some of its key terms and concepts. It will also link to other related topics for more in-depth information.

 

The most important elements of the Process Flow tool are labeled in the following image:

 

 

 

 

## Process Flow View

 

The Process Flow view is the main workspace where you will build your process flow. You can create a new process flow by clicking the Process Flow button on the main toolbar or by using the Toolbox. See Using the Toolbox for more information.

 

 

 

## Process Flow Pane

 

When you first create a new process flow, the Process Flow pane will appear next to the Model pane. Every time you create a new process flow, it will show up as its own tabbed pane. See Arranging Windows and Tabs for more information about rearranging the display of process flow tabs.

 

 

 

## Token

 

The most basic component of a process flow is the *token*, as shown in the following image:

 

 

Tokens are objects that flow through the activities in a process flow during a simulation run. They function very similarly to flow items in a standard 3D model. Like flow items, tokens move from one activity to the next, much like a flow item moving from a source to a queue to a processor, and so forth. But unlike flow items, tokens do not necessarily have to represent a physical object moving through a business system. Tokens can be more abstract—meaning they can represent anything you want them to represent. For example, they can represent a customer's order being placed, a call to a call center, a logical grouping of pallets that are eventually loaded onto a truck for delivery, etc. Tokens will often be logically linked to physical objects in a standard system (and to flow items in a standard 3D model), but they do not have to be. It is essentially up to you to define the nature of the links between tokens and the physical elements in your simulation model.

 

At its most basic level, a token is just a bundle of data that is moving through a process flow. Each token can contain the following basic information:

 

- **ID** - When a token is created using one of the token creation activities (one of the source activities), it is automatically assigned a unique ID number that can be used as a reference point. The ID number will be unique among all process flow objects and instances within a FlexSim model.
- **Name** - Naming your tokens can help you to better identify their purpose, function, or location in the process flow. Tokens are not required to have a name but can be given one when they are created by one of the process flow source activities. You could also possibly change the name at a later stage in the process flow using a Custom Code activity with the Setname picklist option or the `setname()` command.
- **Labels** - Labels are essential to building a complex and dynamic process flow. Labels store custom information on a token that can be used to affect what happens to that token as it advances through a process flow. Various activities might assign data to a token's labels as it moves through the activities in a flowchart. See Key Concepts About Labels for more information about using labels generally.

 

Tokens will only show up in a process flow when a simulation model is running. See Key Concepts About Running a Simulation for more information about how tokens will function and appear visually during a simulation run.

 

 

 

## Library

 

When a process flow view is active, the Library will change to display all the activities and shared assets that can be added to a process flow. You can drag activities directly into the process flow to add them. See Adding and Connecting Activities for more information about additional ways to add activities to a process flow.

 

See Overview of Process Flow Activities and Shared Assets for a high-level summary of each of the process flow activities available in the Library.

 

 

 

## Activity

 

An *activity* is a logical operation or step in a process flow. As such, activities are the basic building blocks of any process flow. The following image shows an example of a typical activity (a Source):

 

 

Activities are dragged from the Library into a process flow and linked together with connectors. As a token enters an activity, it performs the logic associated with that type of activity. This can include assigning labels, moving flowitems, delaying a token, etc.

 

In some activities, like the Assign Labels activity, the token will start and end the activity with no time passing in the simulation clock. While running a model the token will not be visible in that activity. However, using the Step button on the simulation control bar, you can see the token move through each activity one step at a time. In other activities like the Acquire Resource, the token may be delayed and wait in that activity until certain criteria are met.

 

See Overview of Process Flow Activities and Shared Assets for a high-level summary of each of the process flow activities.

 

 

 

## Shared Asset

 

A shared asset is a finite resource that tokens may claim or release at certain points in the process flow. Although they are very similar to activities, there are a few subtle differences. By default, they have a different color than other activities, as shown in the following image:

 

 

Shared assets can impose constraints on the tokens by making the token wait if the requested asset is unavailable. A real-life example of this would be a certain tool that is shared by three different work stations. If one station needs the tool while it is already claimed by another station, that station must wait until the tool becomes available. In that same vein, if a token needs an asset in order to move on to the next activity, the token will wait at its current activity until the asset is available.

 

Currently, there are three types of shared assets:

 

- **Resource** - Represents a limited supply of some resource that can be acquired and released. It can be used to simulate a supply of goods, services, time, materials, employees, etc.
- **List** - Represents a list of tokens, flow items, task executers, task sequences, numbers, strings, etc. Process flows can use a list that is local to the process flow itself or could be tied to a global list in the simulation model.
- **Zone** - Can collect statistical information not available for standard activities. It can also restrict access to a section of the process flow based on certain statistics or other criteria.

 

 

 

## Connector

 

A *connector* is a connection between two activities, as shown in the following image:

 

 

During a simulation run, tokens will use connectors to go from one activity to the next downstream activity. Some activities allow more than one outgoing connector, but many do not. See Adding and Connecting Activities for more information.

  

#### Connectors are not quite the same as port connections

 

Although connectors function very similarly to port connections in the 3D model, they do not function exactly the same. For instance, there is no First Available option in process flow. Tokens can always be sent through any connector at any time as activities cannot be blocked.

  

 

 

## Block (Stacked Block)

 

Occasionally this manual will use the term *stacked block* or *block* to describe a set of activities that have been snapped together into one movable block, as shown in the following image:

 

 

Stacked blocks let you easily link a set of activities into a single sequence of steps, making it easy to move and edit them as if they were one block. When activities are linked together in a stacked block, they are automatically connected as though each activity were connected with connectors.

 

When a token enters a stacked block, it will always start at the topmost activity and then work its way down to the bottom activity. Any connections into a stacked block will be connected to the topmost activity, regardless of where the connection points to in the stacked block. Any outgoing connections from the stacked block will be connected from the bottom activity.

 

 

 

## Quick Properties

 

You can use the settings that appear in the Quick Properties pane to change the basic functionality of the process flows and activities. You can also use the Quick Properties pane to view important information about a token while it is selected. See the Editing Activity Properties for more information.    

## Display Objects

 Although they are not pictured in the image above, display objects are used as presentation tools to convey information about the process flow without actually defining any simulation logic. Display objects may group blocks together and convey flow charting concepts, as in a Flow Chart object, or they may show text, images or arrows. See Process Flow Visuals for more information.
