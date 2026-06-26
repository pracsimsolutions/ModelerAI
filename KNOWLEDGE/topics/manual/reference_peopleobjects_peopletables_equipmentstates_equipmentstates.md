---
id: reference_peopleobjects_peopletables_equipmentstates_equipmentstates
name: "Equipment States"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","EquipmentStates","EquipmentStates"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/EquipmentStates/EquipmentStates.html"
---

# Equipment States

## Overview

 

 

The Equipment States table specifies how long each member of the Equipment Group spends in one of the Equipment States, specified in the People Settings object.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

At model time zero, one row is added for each member of the Equipment Group.

 

 

 

## Columns

 

- **Equipment** - This column records the id Equipment object on this row.
- **Default States** - These columns record the amount of time that the Equipment object has spent in one of the default states. The name of the column is the Display Name specified by the People Settings object.
- **Additional States** - These columns record the amount of time that the Equipment object has spent in one of the additional states. The name of the column is the name of the state specified in the People Settings object.
- **UtilizedTime** - This column records the total time spent in a state marked as Utilized in the People Settings object.
- **TotalTime** - This column records the total time spent in any state specified in the People Settings object.
- **ExcludedTime** - This column records the total time spent in a state marked as Excluded in the Peopl Settings object.
- **Utilization** - This column records the utilization as a percentage. The value is calculated as: UtilizedTime / (TotalTime - ExcludedTime).

 

 

 

## Value Changes

 

The Equipment column does not change after time zero. All other columns update continuously at the model runs.

 

 

 

## Warmup Behavior

 

At the warmup time, the state data on each Equipment object is reset to zero. Consequently, this table will show zeros in all state-based columns at the warmup time.
