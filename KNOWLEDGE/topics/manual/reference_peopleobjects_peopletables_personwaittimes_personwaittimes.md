---
id: reference_peopleobjects_peopletables_personwaittimes_personwaittimes
name: "Person Wait Times"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PersonWaitTimes","PersonWaitTimes"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PersonWaitTimes/PersonWaitTimes.html"
---

# Person Wait Times

## Overview

 

 

The Person Wait Times table records the amount of time that each flowitem created with the Create Person activity spent waiting for each of the following resources:

 

- Location Resources
- Staff Resources
- Transport Resources
- Equipment Resources

 

It is possible for a flowitem to wait for more than one kind of resource simultaneously. This means that the time spent waiting for all resources may exceed the total time the flowitem spent in the model.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

  

#### Time Units

 

All times are recorded in seconds, regardless of model units. This is true for all People Statistics Tables.

  

 

 

## Rows

 

This table adds a new row each time a flowitem is created with the Create Person activity.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that was created with Create Person.
- **Location** - This column records the total time the flowitem waited for a Location Resource.
- **Staff** - This column records the total time the flowitem waited for a Staff Resource.
- **Transport** - This column records the total time the flowitem waited for a Transport Resource.
- **Equipment** - This column records the total time the flowitem waited for a Equipment Resource.

 

 

 

## Value Changes

 

The PID column does not change, once the row is added. The other columns show the total time a flowitem has waited for a specific type of resource. This value continuously updates until the flowitem leaves the model.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to flowitems that are no longer in the model. The data in the remaining rows will remain unaffected.
