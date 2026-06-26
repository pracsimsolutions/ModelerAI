---
id: getmodelunit
name: "getmodelunit"
kind: command
module: ""
signature: "getmodelunit(num querytype) -> num/str/node"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: getmodelunit."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getmodelunit

```flexscript
getmodelunit(num querytype) -> num/str/node
```

Returns information about the model's unit settings.
FlexSim's typical units are: time - 1 second, length - 1 meter, volume - 1 liter, mass - 1 kilogram.
Also may return a name or abbreviation of the current model's units
querytype can be one of the following values:
TIME_MULTIPLE returns the number of seconds represented by each time unit in the current model
LENGTH_MULTIPLE returns the number of meters represented by each length unit in the current model
FLUID_MULTIPLE returns the number of liters represented by each fluid unit in the current model
FLUID_TYPE returns 0 if the fluid units represent volume, or returns 1 if the fluid units represent mass
DATE_TIME_NODE returns a reference to the date/time node in the model units tree
START_TIME returns the start time in seconds (since Jan 1, 1601)
START_TIME_NODE returns a reference to the start time node in the model units tree
CURRENT_TIME returns the current time in seconds (since Jan 1, 1601)
CURRENT_TIME_NODE returns a reference to the current time node in the model units tree
STOP_TIME returns the next stop time in seconds (since Jan 1, 1601)
STOP_TIME_NODE returns a reference to the stop times node in the model units tree
START_YEAR returns the year associated with the start time as a number
START_MONTH returns the month associated with the start time as a number betweeen 1 (Jan) and 12 (Dec)
START_DAY returns the day associated with the start time as a number, where 1 indicates the first day of the month
START_DAYOFWEEK returns the day associated with the start time as a number, where 0 indicates Sunday
START_HOUR returns the hour associated with the start time as a number between 0 and 23
START_MINUTE returns the minute associated with the start time as a number between 0 and 59
START_SECOND returns the second associated with the start time as a number between 0 and 59
START_MILLISECOND returns the millisecond associated with the start time as a number between 0 and 999
CURRENT_YEAR returns the number of years since the model start time
CURRENT_DAY returns the number of days since the model start time
CURRENT_HOUR returns the number of hours since the model start time
CURRENT_MINUTE returns the number of minutes since the model start time
CURRENT_SECOND returns the number of seconds since the model start time
CURRENT_MILLISECOND returns the number of milliseconds since the model start time
CURRENT_YEAR_OF_TIME returns the year associated with the current time as a number
CURRENT_MONTH_OF_YEAR returns the month associated with the current time as a number between 1 (Jan) and 12 (Dec)
CURRENT_DAY_OF_MONTH returns the day associated with the current time as a number, where 1 indicates the first day of the month
CURRENT_DAY_OF_WEEK returns the day associated with the current time as a number, where 0 indicates Sunday
CURRENT_HOUR_OF_DAY returns the hour associated with the current time as a number between 0 and 23
CURRENT_MINUTE_OF_HOUR returns the minute associated with the current time as a number between 0 and 59
CURRENT_SECOND_OF_MINUTE returns the second associated with the current time as a number between 0 and 59
CURRENT_MILLISECOND_OF_SECOND returns the millisecond associated with the current time as a number between 0 and 999
TIME_NAME returns the name of the model time units
TIME_PLURAL_NAME returns the plural name of the model time units
TIME_ABBREVIATION returns the abbreviation of the model time units
LENGTH_NAME returns the name of the model length units
LENGTH_PLURAL_NAME returns the plural name of the model length units
LENGTH_ABBREVIATION returns the abbreviation of the name of the model length units
FLUID_NAME returns the name of the model fluid units
FLUID_PLURAL_NAME returns the plural name of the model fluid units
FLUID_ABBREVIATION returns the abbreviation of the name of the model fluid units
START_TIME_STR returns the start time as a string, formatted by the date and time formats
CURRENT_TIME_STR returns the current time as a string, formmatted by the date and time formats
STOP_TIME_STR returns the next stop time as a string, formatted by the date and time formats
TIME_FORMAT returns the time format used to format date/time strings
DATE_FORMAT returns the date format used to format date/time strings

## Example

```flexscript
getmodelunit(LENGTH_MULTIPLE)
```

