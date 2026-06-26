---
id: reference_processflowobjects_preemption_savetokencontext_savetokencontext
name: "Save Token Context"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Preemption","SaveTokenContext","SaveTokenContext"]
tags: ["manual","reference","processflowobjects","preemption"]
source: "manual/Reference/ProcessFlowObjects/Preemption/SaveTokenContext/SaveTokenContext.html"
---

# Save Token Context

## Overview and Key Concepts

 

The Save Token Context activity saves the current activity context of one or more tokens to a user-defined label on those tokens. The saved context includes the token's current activity, as well as any activity-related data. For example, if a token is in a Delay activity, it will save off the total delay time as well as how far into the delay the token is currently.The Save Token Context activity is used for process flow preemption. See Preemption for information on how to implement preemption.

 

 

 

 

## Connectors

 

The Save Token Context activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Save Token Context activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Token(s)

 

Define the target token(s) whose context you want saved. See Connecting Preempting Process Flows for more information.

 

### Save To Label Name

 

Here you enter the name of the label you want to save the context on. This label will be saved on the target token(s) (not necessarily the token executing the Save Token Context activity).
