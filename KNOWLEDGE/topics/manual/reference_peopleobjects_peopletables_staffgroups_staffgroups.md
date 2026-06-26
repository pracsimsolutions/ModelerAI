---
id: reference_peopleobjects_peopletables_staffgroups_staffgroups
name: "Staff Groups"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","StaffGroups","StaffGroups"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/StaffGroups/StaffGroups.html"
---

# Staff Groups

## Overview

 

 

The Staff Groups table shows each object and sub-group within the Staff Group. The table indicates whether an object is a member of a sub-group.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

At model time zero, one row is added per member of the Staff Group.

 

 

 

## Columns

 

- **Staff** - This column records the id of each member of the Staff group.
- **Group Columns** - These columns appear if there are sub-groups within the Staff Group. There is one column per sub-group. The value in these columns indicates whether the Staff object is a member of the sub-group.

 

 

 

## Value Changes

 

After the initial time-zero event, the values in this table do not change.

 

 

 

## Warmup Behavior

 

This table does not change at the warmup time.
