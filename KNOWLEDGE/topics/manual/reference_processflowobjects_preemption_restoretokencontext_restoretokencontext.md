---
id: reference_processflowobjects_preemption_restoretokencontext_restoretokencontext
name: "Restore Token Context"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Preemption","RestoreTokenContext","RestoreTokenContext"]
tags: ["manual","reference","processflowobjects","preemption"]
source: "manual/Reference/ProcessFlowObjects/Preemption/RestoreTokenContext/RestoreTokenContext.html"
---

# Restore Token Context

## Overview and Key Concepts

 

The Restore Token Context activity restores one or more tokens to a saved activity context. The Restore Token Context activity is used in process flow preemption. See Preemption for information on how to implement preemption.

 

 

 

 

## Connectors

 

The Restore Token Context activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Restore Token Context activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Token(s)

 

Define the target token(s) whose saved context you want restored. This should usually match the **Token(s)** field of a Save Token Context activity. See Connecting Preempting Process Flows for more information.

 

### Saved Label Name

 

Define the name of the label that has context information saved on it. This should match the label name defined in a previous Save Token Context activity.
