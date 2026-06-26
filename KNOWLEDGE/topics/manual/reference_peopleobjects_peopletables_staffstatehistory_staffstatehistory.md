---
id: reference_peopleobjects_peopletables_staffstatehistory_staffstatehistory
name: "Staff State History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","StaffStateHistory","StaffStateHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/StaffStateHistory/StaffStateHistory.html"
---

# Staff State History

## Overview

 

 

The Staff State History table records time-in-state information for every member of the Staff group, for each 15-minute interval of time. This table includes time in each Staff state, time in each Availability State, and time in each combination of Staff/Availability state. For more information on the Staff states, see the People Settings topic.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds one row per member of the Staff group, every 15 minutes.

 

 

 

## Columns

 

- **Staff** - This column records the id of the Staff object on this row.
- **StartTime** - This column records the time at the start of each 15 minute interval.
- **Staff States** - The table contains one of these columns for each of the Staff States specified in the PeopleSettings object. Each column shows the total time (in seconds) the object has spent in that state since the interval began.
- **Availability States** - The table contains one of these columns for each of the Availability states (Available, Acquired, Unavailable). Each column shows the total time (in seconds) the object has spent in that state since the interval begain.
- **Combined States** - The table contains one of these columns for each unique combination of Staff state and Availability state. For example, Idle_Available is a different state than Idle_Acquired. Each column shows the total time (in seconds) the object has spent in that combination of states since the interval begain.
- **Duration** - This column records the total time spent in the interval to date.
- **StateUtilization** - This column shows the utilization of the object for the interval, based on the Staff states.
- **AvailabilityUtilization** - This column shows the utilization of the object for the interval, based on the Availability states.
- **CombinedUtilization** - This column shows the utilization of the object for the interval, based on the combination of Staff and Availability states.

 

 

 

## Value Changes

 

The Staff and StartTime columns are set when the row is added. All other columns show the current value, if the interval has not ended, or the final value for the interval if it has ended.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to completed intervals. All columns (except the Staff column) are reset as if the interval started over. However, the interval timing will continue to be based on the model start time.
