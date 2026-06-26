---
id: reference_processflowobjects_coordination_join_join
name: "Join"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Coordination","Join","Join"]
tags: ["manual","reference","processflowobjects","coordination"]
source: "manual/Reference/ProcessFlowObjects/Coordination/Join/Join.html"
---

# Join

## Overview and Key Concepts

 

 

 

Tokens wait at the Join activity until one token arrives from each incoming connector. The token from the first connector (rank 1) is then released out the outgoing connector and the other tokens are destroyed.

 

 

Specifying a Partition ID makes tokens wait only for other tokens in the same partition. This is useful for coordinating with other tokens of the same type. See Coordination for information on coordinating tokens.

 

 

 

## Connectors

 

The Join activity only allows one connector out.

 

The number of incoming connectors will affect the behavior of this activity. See Connectors and Coordination Activities for more about the unique behavior of connectors in Coordination activities.

 

 

 

## Properties

 

The following image shows properties for the Join activity:

 

 

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
