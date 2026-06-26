---
id: modellogic_labels_trackingmanaginglabels_trackingmanaginglabels
name: "Tracking and Managing Labels"
kind: manual
breadcrumb: ["ModelLogic","Labels","TrackingManagingLabels","TrackingManagingLabels"]
tags: ["manual","modellogic","labels","trackingmanaginglabels"]
source: "manual/ModelLogic/Labels/TrackingManagingLabels/TrackingManagingLabels.html"
---

# Tracking and Managing Labels

## Reasons to Track and Manage Labels

 

If your system of labels is complex, you'll want to test and validate your labels to make sure that they are functioning the way you intended. You should also check that any label-dependent logic works correctly in the system. The best way to test this is to run a simulation model and track the value of labels on flow items, tokens, or 3D objects as they interact with the simulation model.

 

This topic will explain how to use various tools to track labels.

 

 

 

## Viewing a Flow Item's Properties

 

To validate flow item labels, you'll need to view their labels on their properties window:

 

1. Run the simulation long enough for flow items to interact with the simulation model. Pause the model by pressing the **Stop** button on the simulation control bar.
1. Click a flow item to open its properties on the right.
1. You can view the flow item's current label values under the **Labels** section.

 

 

 

## Viewing a 3D Object's Labels

 

To validate the labels on a 3D object (such as a fixed resource or task executer):

 

1. Run the simulation long enough for the 3D object to interact with the simulation model. When the 3D object gets to a point where its labels might change, pause the model by pressing the **Stop** button on the simulation control bar.
1. Click the 3D object to open its properties on the right.
1. You can view the 3D object's current label values under the **Labels** section.

 

 

 

## The View Tokens Tool

 

The View Tokens tool allows you to see the value of a label on all the tokens that are currently in the process flow. It can also visually trace a specific token as it moves through the process flow. See Troubleshooting Process Flows for more information about tracing tokens.

 

To view the labels on tokens using the View Tokens tool:

 

1. Run the simulation long enough for several tokens to interact with the simulation model. When the token gets to a point where its labels might change, pause the model by pressing the **Stop** button on the simulation control bar.
1. Click a blank area in a process flow to ensure that nothing is selected.
1. In Quick Properties, click the **View Tokens** button.
1. Click the **Arrange By** menu and select a method for displaying and organizing tokens.
1. In the **Token** tab list on the bottom left, click a token you want to view. That token's details will appear in the **Label** tab on the bottom right.

 

 

 

## The View Activities Tool

 

The View Activities tool can search for all process flow activities that use a particular label. This tool can help you get an overall perspective on how a label is being used throughout an entire process flow. You can also edit an activity's properties directly in this window.

 

To search for a particular label to find all the activities that use that label:

 

1. Click a blank area in a process flow to ensure that nothing is selected.
1. In Quick Properties, click the **View Activities** button.
1. In the **Filter Activities** group, next to the menu that says **By Label**, type the name of the label you want to find.
1. The **Activities** list on the bottom left will filter itself so that only activities which use this label will be displayed.
1. If needed, you can click an activity in the **Activities** list to edit a specific activity's properties.

 

 

 

## Token Properties

 

During a simulation run, you can double-click a token in a process flow to view its properties window. Its Labels tab will display the current values of the labels on the token:

 

 

 

 

## Displaying Label Values on Tokens

 

You can change tokens to display the value of labels on tokens. Tokens can display a label's value as text on the token during a simulation run. You can also change the color or shape of a token based on a label value during a simulation run. See Changing Process Flow Visuals for more information.

 <!-- Comment from Jordan to work in later: It took me a bit to understand what this was talking about. Maybe if the title was "Changing Flowitem or Token Appearance Based on Label Values", it would make more sense. Plus, you could mention changing a flowitem's color base on label value using the pick options. My thought: maybe breaking it up and providing images as examples would help -->
