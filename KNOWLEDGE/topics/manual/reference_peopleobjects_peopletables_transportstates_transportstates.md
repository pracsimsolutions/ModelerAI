---
id: reference_peopleobjects_peopletables_transportstates_transportstates
name: "Transport States"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","TransportStates","TransportStates"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/TransportStates/TransportStates.html"
---

# Transport States

## Overview

 

 

The Transport States table specifies how long each member of the Transports Group spends in one of the Transport States, specified in the People Settings object.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

At model time zero, one row is added for each member of the Transports Group.

 

 

 

## Columns

 

- **Transport** - This column records the id Transport object on this row.
- **Default States** - These columns record the amount of time that the Transport object has spent in one of the default states. The name of the column is the Display Name specified by the People Settings object.
- **Additional States** - These columns record the amount of time that the Transport object has spent in one of the additional states. The name of the column is the name of the state specified in the People Settings object.
- **UtilizedTime** - This column records the total time spent in a state marked as Utilized in the People Settings object.
- **TotalTime** - This column records the total time spent in any state specified in the People Settings object.
- **ExcludedTime** - This column records the total time spent in a state marked as Excluded in the Peopl Settings object.
- **Utilization** - This column records the utilization as a percentage. The value is calculated as: UtilizedTime / (TotalTime - ExcludedTime).

 

 

 

## Value Changes

 

The Transport column does not change after time zero. All other columns update continuously at the model runs.

 

 

 

## Warmup Behavior

 

At the warmup time, the state data on each Transport object is reset to zero. Consequently, this table will show zeros in all state-based columns at the warmup time.
