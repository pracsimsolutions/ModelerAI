---
id: reference_peopleobjects_peopletables_linestaytimehistory_linestaytimehistory
name: "Line Staytime History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","LineStaytimeHistory","LineStaytimeHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/LineStaytimeHistory/LineStaytimeHistory.html"
---

# Line Staytime History

## Overview

 

 

The Line Staytime History records the entry and exit times for each flowitem as they enter and exit Waiting Line objects.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a new row when a flowitem (created with the Create Person activity) enters a Waiting Line object.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that entered the line.
- **Line** - This column records the id of the line that the flowitem entered.
- **EnterTime** - This column records the time that the flowitem entered the line.
- **Exit Time** - This column records the time that the flowitem exited the line.

 

 

 

## Value Changes

 

The ExitTime column is set to the current time when the flowitem exits the Line. All other values are set when the row is added.

 

 

 

## Warmup Behavior

 

At the warmup time, this table removes all rows that have a value in the ExitTime column. The data in the remaining rows is unaffected.
