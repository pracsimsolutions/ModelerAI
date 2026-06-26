---
id: reference_peopleobjects_peopletables_equipmenttraveldistance_equipmenttraveldistance
name: "Equipment Travel Distance"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","EquipmentTravelDistance","EquipmentTravelDistance"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/EquipmentTravelDistance/EquipmentTravelDistance.html"
---

# Equipment Travel Distance

## Overview

 

 

The Equipment Travel Distance table records how far each member of the Equipment Group has travelled during the model run.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

  

#### Distance Units

 

Distance is recorded in meters, regardless of model units. This is true for all People Statistics Tables.

  

 

 

## Rows

 

At model time zero, one row is added for each member of the Equipment Group.

 

 

 

## Columns

 

- **Equipment** - This column records the id of the Equipment object.
- **Distance** - This column records the distance travelled by each Equipment object during the model run.

 

 

 

## Value Changes

 

The Equipment column does not change after time zero. The Distance column updates whenever the Equipment object is used in any of the following activities: 

- Escort Person
- Wait In Line
- Move Equipment

 

 

 

## Warmup Behavior

 

At the warmup time, this table resets the Distance column to zero.
