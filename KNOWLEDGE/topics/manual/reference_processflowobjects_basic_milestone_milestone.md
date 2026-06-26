---
id: reference_processflowobjects_basic_milestone_milestone
name: "Milestone"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Basic","Milestone","Milestone"]
tags: ["manual","reference","processflowobjects","basic"]
source: "manual/Reference/ProcessFlowObjects/Basic/Milestone/Milestone.html"
---

# Milestone

## Overview and Key Concepts

 

The Milestone activity is used to signal that some object has acheived a milestone. You can place this activity at any important point in your Process Flow, and specify the object that has acheived a milestone.

 

 

The Milestone activity is designed to work with Milestone Collectors. For more information on how to use milestones effectively in your model, see the Using Milestone Collectors topic.

 

This activity allows you to specify the follwing things:

 

1. The Milestone Collector should this activity reference,
1. The name of the Milestone this activity will update, and
1. The involved object.

 

The referenced Milestone Collector will make sure there is a column for the specified milestone. When a token enters this activiy, the Milestone Collector will update the time in that milestone column.

 

You can also specify whether future milestones should be recorded on a new row of the Milestone Collector's table.

 

 

 

## Connectors

 

The Milestone activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Milestone activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Milestone Collector

 

The **Milestone Collector** property determines which Milestone Collector this activity will affect.

 

### Milestone Name

 

The **Milestone Name** property determines which milestone will be updated when a token crosses enters this activity. The referenced Milestone Collector will assert a column with this name.

 

### Involved

 

This property indicates which object has acheived the current milestone. The Milestone collector will assert a row for that object, and update the time in the specified column.

 

### Start new entry on next milestone

 

Check this box to indicate that if the involved object acheives any additional milestones, then the referenced Collector should use a new row to record the milestone time. This option can be used to prevent recirculating items from overwriting their milestone data.
