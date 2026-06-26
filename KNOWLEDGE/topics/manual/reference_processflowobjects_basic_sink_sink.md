---
id: reference_processflowobjects_basic_sink_sink
name: "Sink"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Basic","Sink","Sink"]
tags: ["manual","reference","processflowobjects","basic"]
source: "manual/Reference/ProcessFlowObjects/Basic/Sink/Sink.html"
---

# Sink

## Overview and Key Concepts

 

The Sink activity destroys tokens, removing all data stored on those tokens. If a token has children, it will remain inside the Sink activity until all of its children have been destroyed, thus maintaining the links and data on the children. If **Deallocated Shared Assets** is unchecked and the token has allocated shared assets, the token will also remain inside the Sink activity until all shared assets have been deallocated.

 

  

#### Best Practices for Destroying Tokens

 

The Sink activity and the Finish activity are valid ways to destroy tokens. Do not use the destroyobject() command to destroy tokens.

  

 

 

## Connectors

 

The Sink activity marks the end of a set of activities. No activity may be connected to the end of a Sink. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Sink activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Deallocate Shared Assets

 

If the **Deallocate Shared Assets** box is checked, any Shared Assets that are allocated by the token will be deallocated when the Sink destroys the token. In other words, when a token is destroyed by the Sink, any resources that have been previously acquired by the token will be released and any entries or backorders on lists associated with the token will be deleted. If the token is in a zone, the zone will only update its content and not update any of its other statistics. If unchecked, the token will remain in the Sink activity until its shared assets have been deallocated.

  

#### Exiting a Zone

 

In order to properly remove a token from a zone and have the zone's output, staytime and custom statistics updated, send the token through an Exit Zone activity.
