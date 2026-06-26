---
id: reference_processflowobjects_preemption_releasetoken_releasetoken
name: "Release Token"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Preemption","ReleaseToken","ReleaseToken"]
tags: ["manual","reference","processflowobjects","preemption"]
source: "manual/Reference/ProcessFlowObjects/Preemption/ReleaseToken/ReleaseToken.html"
---

# Release Token

## Overview and Key Concepts

 

The Release Token activity aborts the current activity of one or more tokens and sends them to a new activity.

 

This activity is used in process flow preemption. See Preemption for information on how to implement preemption.

 

 

 

 

## Connectors

 

The Release Token activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Release Token activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Token(s)

 

Define the target token(s) who you want to release. This should usually match the **Token(s)** field of a Save Token Context activity. See Connecting Preempting Process Flows for more information.

 

### Release To

 

Define the activity you want to release the token(s) to. Usually you will use the sampler button to directly choose the destination activity.
