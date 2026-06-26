---
id: reference_peopleobjects_peopletables_persontraveldistance_persontraveldistance
name: "Person Travel Distance"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PersonTravelDistance","PersonTravelDistance"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PersonTravelDistance/PersonTravelDistance.html"
---

# Person Travel Distance

## Overview

 

 

This table records how far each flowitem created with the Create Person activity travels during its time in the model.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

  

#### Distance Units

 

Distance is recorded in meters, regardless of model units. This is true for all People Statistics Tables.

  

 

 

## Rows

 

This table adds a new row each time a flowitem is created with the Create Person activity.

 

 

 

## Columns

 

- **PID** - This column records the id of the flowitem that was created with Create Person.
- **Distance** - This column records the distance that each flowitem travels while in the model.

 

 

 

## Value Changes

 

The PID column is not updated once the row has been added. The Distance column is updated any time the flowitem is involved in one of the following activities: 

- Walk
- Escort Person
- Transport Person
- Wait In Line

 

 

 

## Warmup Behavior

 

At the warmup time, this table will remove the rows corresponding to flowitems that are no longer in the model. The data in the remaining rows will remain unaffected.
