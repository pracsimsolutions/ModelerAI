---
id: reference_peopleobjects_peopletables_equipmentgroups_equipmentgroups
name: "Equipment Groups"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","EquipmentGroups","EquipmentGroups"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/EquipmentGroups/EquipmentGroups.html"
---

# Equipment Groups

## Overview

 

 

The Equipment Groups table shows each object and sub-group within the Equipment Group. The table indicates whether an object is a member of a sub-group.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

At model time zero, one row is added per member of the Equipments Group.

 

 

 

## Columns

 

- **Equipment** - This column records the id of each member of the Equipment group.
- **Group Columns** - These columns appear if there are sub-groups within the Equipments Group. There is one column per sub-group. The value in these columns indicates whether the Equipment object is a member of the sub-group.

 

 

 

## Value Changes

 

After the initial time-zero event, the values in this table do not change.

 

 

 

## Warmup Behavior

 

This table does not change at the warmup time.
