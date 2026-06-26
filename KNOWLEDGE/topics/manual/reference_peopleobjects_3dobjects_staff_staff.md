---
id: reference_peopleobjects_3dobjects_staff_staff
name: "Staff"
kind: manual
breadcrumb: ["Reference","PeopleObjects","3DObjects","Staff","Staff"]
tags: ["manual","reference","peopleobjects","3dobjects"]
source: "manual/Reference/PeopleObjects/3DObjects/Staff/Staff.html"
---

# Staff

## Overview and Key Concepts

 

The Staff object is a specialized Operator designed to work with the Acquire and Release Staff activities.

 

 

 

 

## Events

 

The staff uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

### On Acquire

 

The on acquire event fires when the staff is acquired by an Acquire Staff activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Pulling Token | Token | The token in the Acquire Staff activity. |
| Puller | Object | The object acquiring the staff. |

 

### On Release

 

The on release event fires when the staff is released by a Release Staff activity.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Releasing Token | Token | The token in the Release Staff activity. |

 

This object also uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The staff uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The staff uses the standard properties of an Operator. See Operator Properties for an explanation of these statistics.
