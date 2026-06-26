---
id: reference_processflowobjects_tokencreation_interarrivalsource_interarrivalsource
name: "Inter-Arrival Source"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TokenCreation","InterArrivalSource","InterArrivalSource"]
tags: ["manual","reference","processflowobjects","tokencreation"]
source: "manual/Reference/ProcessFlowObjects/TokenCreation/InterArrivalSource/InterArrivalSource.html"
---

# Inter-Arrival Source

## Overview and Key Concepts

 

The Inter-Arrival Source activity creates new tokens according to a specific interval of time. Similar to the Inter-Arrival Time arrival style of FlexSim's standard Source, you can use a fixed number to set an exact interval of time between token creations or you can use a statistical distribution to randomly calculate the time between arrivals. Once a token is created it will be released to the next activity.

 

 

 

 

## Connectors

 

The Inter-Arrival Source activity only allows one connector out. See Adding and Connecting Activities for more information.

 

  

## Properties

 

The following image shows properties for the Inter-Arrival Source activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Enabled

 

When unchecked, this activity will not schedule events or create any tokens.

 

### Arrival at time 0

 

If checked, a token will be created at time 0 when you start the model run.

 

### Inter-Arrival Time

 

This defines the interval of model time between token creations.

 

### Token Name

 For debugging purposes, you can assign a name to the tokens created by this source. For example, if you used the name *Token1* in this box, all of the tokens created by this source would be assigned this name. Token names are not required so the box is left empty by default.
