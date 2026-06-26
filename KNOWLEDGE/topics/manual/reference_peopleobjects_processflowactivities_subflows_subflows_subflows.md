---
id: reference_peopleobjects_processflowactivities_subflows_subflows_subflows
name: "People Sub Flows"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","SubFlows","SubFlows","SubFlows"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/SubFlows/SubFlows/SubFlows.html"
---

# People Sub Flows

## Overview and Key Concepts

 

 

People Sub Flow activities are tied to a pre-built Sub Flow Process Flow object. These activities create a new child token, assign new labels to this token, and then put the token in the Start activity of its associated Process Flow.

 

The Process Flow tied to the activity is known as its Inner Flow and can be viewed by choosing the Inner Flow option from the activity's context menu.

 

 

 

 

## Inner Flow

 

 

The Inner Flow is the Process Flow tied to the activity. It contains a set of activities that define the logic of the activity. The properties for these activities are evaluated and each value is stored on the new token as a label on the new token that is then used in the Process Flow.

 

The Inner Flow of an activity can be viewed, but not modified. The intention is to allow users to view the logic of the activity so they can better understand the logic of the activity, debug unwanted behavior, and even copy the Inner Flow into a model where modifications can be made.
