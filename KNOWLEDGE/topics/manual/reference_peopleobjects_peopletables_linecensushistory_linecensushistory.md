---
id: reference_peopleobjects_peopletables_linecensushistory_linecensushistory
name: "Line Census History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","LineCensusHistory","LineCensusHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/LineCensusHistory/LineCensusHistory.html"
---

# Line Census History

## Overview

 

 

The Line Census History table records the entry and exits events for every flowitem in the model as they enter and exit Waiting Line objects.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a row every time a flowitem (created with the Create Person activity) enters or exits Waiting Line object.

 

 

 

## Columns

 

- **Time** -This column records the time of the entry or exit.
- **PID** - This column records the id of the flowitem that entered or exited.
- **Line** - This column records the Waiting Line that was entered or exited.
- **InOut** - This column records a 1 if the line was entered. If the Line was exited, this column records a -1.

 

 

 

## Value Changes

 

Once a row is added, the values in that row do not change.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove all rows except the rows that record a flowitem entering its current Waiting Line. The data on these rows remains unchanged.
