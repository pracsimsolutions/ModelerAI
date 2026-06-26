---
id: reference_peopleobjects_peopletables_locationcensushistory_locationcensushistory
name: "Location Census History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","LocationCensusHistory","LocationCensusHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/LocationCensusHistory/LocationCensusHistory.html"
---

# Location Census History

## Overview

 

 

The Location Census History table records the entry and exits events for every flowitem in the model as they enter and exit Location objects.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a row every time a flowitem (created with the Create Person activity) enters or exits a Location in the Locations Group.

 

 

 

## Columns

 

- **Time** -This column records the time of the entry or exit.
- **PID** - This column records the id of the flowitem that entered or exited.
- **Location** - This column records the location that was entered or exited.
- **InOut** - This column records a 1 if the location was entered. If the location was exited, this column records a -1.

 

 

 

## Value Changes

 

Once a row is added, the values in that row do not change.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove all rows except the rows that record a flowitem entering its current Location. The data on these rows remains unchanged.
