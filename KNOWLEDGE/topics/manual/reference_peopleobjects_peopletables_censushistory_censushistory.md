---
id: reference_peopleobjects_peopletables_censushistory_censushistory
name: "Census History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","CensusHistory","CensusHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/CensusHistory/CensusHistory.html"
---

# Census History

## Overview

 

 

The Census History table records the creation and removal events of every flowitem in the model.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a row each time a flowitem is created with the Create Person activity, or when the flowitem is removed with the Remove Person activity.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that was created or removed.
- **Time** - This column records the time that the flowitem was created or removed.
- **InOut** - This column records a 1 if the flowitem was created, and a -1 if it was removed.

 

 

 

## Value Changes

 

Once a row is added, none of the values in this table change.

 

 

 

## Warmup Behavior

 

At the warmup time, this table removes all rows that correspond to flowitems that have left the model. The data in all remaining rows will not be changed.
