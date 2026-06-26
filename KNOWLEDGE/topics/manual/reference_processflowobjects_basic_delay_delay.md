---
id: reference_processflowobjects_basic_delay_delay
name: "Delay"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Basic","Delay","Delay"]
tags: ["manual","reference","processflowobjects","basic"]
source: "manual/Reference/ProcessFlowObjects/Basic/Delay/Delay.html"
---

# Delay

## Overview and Key Concepts

 

The Delay activity will hold the token for a certain length of time. You can use a fixed time or you can create the delay time dynamically using a label value on a token, a statistical distribution, etc. If you are planning to use the Delay activity as a *breathe*, use the Breathe Activity instead.

 

  

#### Using Delays as Breathes

 

A *breathe*, which is a delay time of 0, is similar to sending a delayed message in 0 time. Doing so will create an event in 0 time before releasing the token. Breathes can allow you to break up certain operations to ensure that things are happening in an appropriate order. This is like taking a breath before moving on. It may be difficult to know when this is needed, but if something in your model is not behaving properly, adding a breathe can potentially solve issues.

  

 

 

## Connectors

 

The Delay activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Delay activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Delay Time

 

The **Delay Time** defines the amount of time that the token will be delayed.
