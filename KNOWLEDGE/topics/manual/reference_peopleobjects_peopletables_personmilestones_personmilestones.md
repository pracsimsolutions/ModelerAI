---
id: reference_peopleobjects_peopletables_personmilestones_personmilestones
name: "Person Milestones"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PersonMilestones","PersonMilestones"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PersonMilestones/PersonMilestones.html"
---

# Person Milestones

## Overview

 

 

The PersonMilestones table records the time at which a flowitem created with the Create Person activity was created and removed. This table also records the times that the flowitem arrived at any other Milestone activities that are linked to this table.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a new row each time a flowitem is created with the Create Person activity.

 

 

 

## Columns

 

- **LastMilestoneAcheivedColumn** - This column is present for compatibility with Milestone Collectors.
- **Created** - This column records the time that the flowitem was created.
- **Milestone Columns** - These columns are defined by Milestone activities, like other Milestone Collectors.
- **Removed** - This column records the time that the flowitem was removed with the Remove Person activity.
- **PID** - This column records the id of the flowitem that was created with Create Person.

 

 

 

## Value Changes

 

The LastMilestoneAcheivedColumn, Created, and PID columns are never updated once the row is added. All other columns are updated when the corresponding milestone is acheived.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to flowitems that are no longer in the model. The data in the remaining rows will remain unaffected.
