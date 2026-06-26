---
id: reference_processflowobjects_coordination_synchronize_synchronize
name: "Synchronize"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Coordination","Synchronize","Synchronize"]
tags: ["manual","reference","processflowobjects","coordination"]
source: "manual/Reference/ProcessFlowObjects/Coordination/Synchronize/Synchronize.html"
---

# Synchronize

## Overview and Key Concepts

 

Tokens wait at the Synchronize activity until one token arrives from each incoming connector. This activity is similar to the Join activity, except that there will be an outgoing connector corresponding to each incoming connector. The token from the first incoming connector is then released out the first outgoing connector, the token from the second incoming connector is released out the second outgoing connector, and so on.

 

Specifying a Partition ID makes tokens wait only for other tokens in the same partition. This is useful for coordinating with other tokens of the same type. See Coordination for information on coordinating tokens.

 

 

 

 

## Connectors

 

The Synchronize activity allows any number of connectors out as long as the number of outgoing connectors matches the number of incoming connectors. The Synchronize does not allow other activities to be snapped to it as doing so would remove all connectors from the Synchronize.

 

The number of incoming and outgoing connectors will affect the behavior of this activity. See Connectors and Coordination Activities for more about the unique behavior of connectors in Coordination activities.

 

 

 

## Properties

 

The following image shows properties for the Synchronize activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Partition ID

 

Defines which partition tokens will use.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not pulled its required amount after the specified time. See Max Wait Timer for more information.

 

### Use Max Idle Timer

 

The max idle timer will be evaluated each time a value is pulled from the list by the token. See Max Idle Timer for more information.
