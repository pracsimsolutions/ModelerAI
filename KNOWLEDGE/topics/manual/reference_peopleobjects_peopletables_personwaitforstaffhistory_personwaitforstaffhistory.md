---
id: reference_peopleobjects_peopletables_personwaitforstaffhistory_personwaitforstaffhistory
name: "Person Wait for Staff History"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PersonWaitForStaffHistory","PersonWaitForStaffHistory"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PersonWaitForStaffHistory/PersonWaitForStaffHistory.html"
---

# Person Wait for Staff History

## Overview

 

 

The Person Wait for Staff History table records information about each request for any Staff Resource. This information includes the time the request was made, as well as the time the request was fulfilled.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

 

 

 

## Rows

 

This table adds a row any time a flowitem (created by the Create Person activity) attempts to acquire something from a Staff Resource.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that is requesting a resource.
- **Flow** - This column records the name of the Process Flow containing the Acquire Staff activity.
- **Activity** - This column records the name of activity used to make the request.
- **Priority** - This column records the priority of the request.
- **RequestTime** - This column records the time the request is made.
- **AcquireTime** - This column records the time the request is either fulfilled or cancelled.
- **Staff** - This column records the ID of the Staff that is acquired as a result of the request. If the request was cancelled, this column will contain a null value.

 

 

 

## Value Changes

 

The AcquireTime and Staff columns are set once the request is either fulfilled or cancelled. All other columns are set when the request is created.

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to complete requests. The data in the remaining rows will remain unchanged.
