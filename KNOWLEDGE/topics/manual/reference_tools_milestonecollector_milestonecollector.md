---
id: reference_tools_milestonecollector_milestonecollector
name: "The Milestone Collector"
kind: manual
breadcrumb: ["Reference","Tools","MilestoneCollector","MilestoneCollector"]
tags: ["manual","reference","tools","milestonecollector"]
source: "manual/Reference/Tools/MilestoneCollector/MilestoneCollector.html"
---

# The Milestone Collector

## Overview and Key Concepts

 

A Milestone Collector is a Statistics Collector that helps you track the progress of the objects as they flow through the model.

 

 

The Milestone Collector can be accessed from the Toolbox.

 

## Milestones

 

In FlexSim, a milestone is an ordinary event that is important statistically. For example you might be interested in the time it takes for a flowitem to travel between two parts of a facility. That time span is defined by two times: the time that the flowitem begins travelling, and the time that the flowitem ends travelling. You can create a milestone for each of those events in a Milestone Collector. The milestone collector will then record when each of those events happen for every flowitem, allowing you to gather statistics on those two events. For example, you could determine how the average, maximum, and minimum times between the two milestones.

 

The easiest way to create a Milestone is to use the Milestone activity. You can place the Milestone activity anywhere in a process. When a token arrives at that activity, the Milestone activity will notify the Milestone Collector that an item has acheived a milestone. The the Milestone Collector will record the time that the given item acheived the milestone.

 

You can also create milestones that use events on 3D object. The Milestone collector allows you to add a custom milestone, and define the events that count as acheiving the milestone. When the event occurs, the milestone collector will record the time that the involved item acheived the event.

 

Once you have defined your Milestones, the Milestone Collector will create a table with one column per Milestone. It will also create one additional column that tracks the most recent column updated. When any event on any milestone occurs, the following steps happen: 

1. If that object has never been seen before, the Milestone Collector adds a row for that object.
1. The Milestone Collector determines which column should be updated on that row.
1. The Milestone Collector records the current time in that column.
1. The Milestone Collector records which column was most recently updated.

 As the model runs, this table might look similar to the following:

 

 

In the above table, there is one row per flowitem. You can see the times that each item reached each milestone. The rows closer to the end have null values in some of the columns, indicating that the item for that row has not yet reached those milestones.

 

In some cases, you may want to record additional data about each entity, in addition to its milestone times. You can add additional columns to record that data. These additional columns will be added after the milestone columns.

 

 

 

## Events

 

The Milestone Collector allows you to listen to Standard Events, Group Events, and Tracked Variable Events. For more information, see the Statistics Collector Events Tab topic.

 

 

 

## Properties

 

The Statistics Collector has three tabs with various properties. The properties in these tabs will be explained in more detail in next sections.

 <!-- I know it's a little weird to have this section that only has one paragraph, but I'm trying to be consistent with the way tools and objects with multiple tabs are organized. Normally in this section, I provide links to tabs that objects and tools share in common. On tools that have one tab, I just list all the properties here.--> <!-- Should we talk about the Name box here? I think that's fairly intuitive. What about the Enabled checkbox? --> 

 

 

## The Milestones Tab

 

This tab allows you to create a list of Milestones. For each Milestone, you can also edit the list of events, and configure each individual event, from this tab.

 

The Milestones tab has the following properties:

 

 

### Milestone List

 

Lists the Milestones for this Milestone Collector. When you select a milestone in the list, the area to the right of the list will update with details about the current Milestone.

 

The following table explains the buttons on the milestone list:

 

| Icon | Description |
| --- | --- |
|  | Adds a new Milestone to the list. |
|  | Duplicates the selected Milestone in the Milestone list. |
|  | Removes the selected Milestone from the Milestone list. |
|  | Reorders the selected Milestone in the Milestone list. |

 

### Name

 

Allows you to edit the name of this Milestone

 

### Event List

 

Lists the events that apply to the selected Milestone. The Milestone Collector can listen to:

 

- Standard Events
- Group Events
- Tracked Variable Events

 

The following table explains the buttons on the event list:

 

| Icon | Description |
| --- | --- |
|  | Opens a menu to add a new event to the list. |
|  | Samples an object in the 3D model, a process flow, or toolbox to add it to the event list. |
|  | Duplicates the selected event from the event list. |
|  | Removes the selected event from the event list. |
|  | Reorders the selected event from the event list. |

 For more information on the properties within the **Event Details** panels, see the Statistics Collector Events Tab topic. 

 

 

## The Milestone Activities Tab

 

This tab shows a list of the Milestone activities that are attached to this Milestone Collector. It also allows you to reorder those activities, which will reorder the milestones as they appear in the table.

 

The Milestone Activities tab has the following properties:

 

 

### Milestone Activity List

 

This interface displays the list of Milestone activities that are attached to this collector. You can reorder those milestones using the and buttons.

  

#### Milestone Activity Order

 

More than one activity can update a single milestone. The order of the milestone columns in the table is determined by reading the list of milestone activities, and adding milestone columns as unique milestones are encountered.

  

 

 

## The Data Tab

 

This tab allows you to add columns in addition to the milestone time columns. For example, you could record the type of each item.

 

The Data tab has the following properties:

 

 

### Addional Columns

 

This interface is almost identical to adding columns to a regular Statistics Collector, except this interface does not allow you to add a Column Set. For more information, see the Statistics Collector Columns Tab topic.

 

### Reuse Untracked Rows

 

Check this box to allow the Milestone Collector to reuse a row if the involved item for that row is no longer being tracked, rather than adding a new row. This can greatly reduce the number of rows in this table.

 

 

 

## The General Tab

 

The General tab is identical to the Statistics Collector Properties General Tab. For more information, see the Statistics Collector Properties General Tab topic.
