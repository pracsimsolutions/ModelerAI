---
id: reference_peopleobjects_peopletables_personstates_personstates
name: "Person States"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PersonStates","PersonStates"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PersonStates/PersonStates.html"
---

# Person States

## Overview

 

 

The Person States table records the time a flowitem, created with the Create Person activity, spends in one of the Person States, specified in the People Settings object.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

  

#### Time Units

 

All times are recorded in seconds, regardless of model units. This is true for all People Statistics Tables.

  

 

 

## Rows

 

Rows are added whenever a new flowitem is created with the Create Person activity.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that was created with Create Person.
- **Default States** - These columns record the amount of time that the flowitem has spent in one of the default states. The name of the column is the Display Name specified by the People Settings object.
- **Additional States** - These columns record the amount of time that the flowitem has spent in one of the additional states. The name of the column is the name of the state specified in the People Settings object.
- **UtilizedTime** - This column records the total time spent in a state marked as Utilized in the People Settings object.
- **TotalTime** - This column records the total time spent in any state specified in the People Settings object.
- **ExcludedTime** - This column records the total time spent in a state marked as Excluded in the Peopl Settings object.
- **Utilization** - This column records the utilization as a percentage. The value is calculated as: UtilizedTime / (TotalTime - ExcludedTime).

 

 

 

## Value Changes

 

Values in the PID column do not change. Values in all other columns always reflect the time spent in the various states. When the flowitem leaves the model, these columns will show the total time spent in each state, as well as the associated totals and utilization.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to flowitems that are no longer in the model. The data in the remaining rows will remain unaffected.
