---
id: reference_peopleobjects_peopletables_locationstaytimehistory_locationstaytimehistory
name: "Location Staytime History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","LocationStaytimeHistory","LocationStaytimeHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/LocationStaytimeHistory/LocationStaytimeHistory.html"
---

# Location Staytime History

## Overview

 

 

The Location Staytime History records the entry and exit times for each flowitem as they enter and exit Location objects.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a new row when a flowitem (created with the Create Person activity) enters a Location in the Locations Group.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that entered the Location.
- **Location** - This column records the id of the Location that the flowitem entered.
- **EnterTime** - This column records the time that the flowitem entered the location.
- **Exit Time** - This column records the time that the flowitem exited the location.

 

 

 

## Value Changes

 

The ExitTime column is set to the current time when the flowitem exits the Location. All other values are set when the row is added.

 

 

 

## Warmup Behavior

 

At the warmup time, this table removes all rows that have a value in the ExitTime column. The data in the remaining rows is unaffected.
