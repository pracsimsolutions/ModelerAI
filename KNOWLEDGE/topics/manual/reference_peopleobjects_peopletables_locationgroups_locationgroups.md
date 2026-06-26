---
id: reference_peopleobjects_peopletables_locationgroups_locationgroups
name: "Location Groups"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","LocationGroups","LocationGroups"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/LocationGroups/LocationGroups.html"
---

# Location Groups

## Overview

 

 

The Location Groups table shows each object and sub-group within the Locations Group. The table indicates whether an object is a member of a sub-group.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

At model time zero, on row is added per member of the Locations Group.

 

 

 

## Columns

 

- **Location** - This column records the id of each member of the Locations Group.
- **Group Columns** - These columns appear if there are sub-groups within the Locations Group. There is one column per sub-group. The value in these columns indicates whether the Location object is a member of the sub-group.

 

 

 

## Value Changes

 

After the initial time-zero event, the values in this table do not change.

 

 

 

## Warmup Behavior

 

This table does not change at the warmup time.
