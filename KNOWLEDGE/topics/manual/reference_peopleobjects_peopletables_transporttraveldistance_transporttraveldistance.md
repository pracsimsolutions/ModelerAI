---
id: reference_peopleobjects_peopletables_transporttraveldistance_transporttraveldistance
name: "Transport Travel Distance"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","TransportTravelDistance","TransportTravelDistance"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/TransportTravelDistance/TransportTravelDistance.html"
---

# Transport Travel Distance

## Overview

 

 

The Transport Travel Distance table records how far each member of the Transports Group has travelled during the model run.

 

For more general information on People Statistics Tables, see the People Statistics Tables topic.

  

#### Distance Units

 

Distance is recorded in meters, regardless of model units. This is true for all People Statistics Tables.

  

 

 

## Rows

 

At model time zero, one row is added for each member of the Transports Group.

 

 

 

## Columns

 

- **Transport** - This column records the id of the Transport object.
- **Distance** - This column records the distance travelled by each Transport object during the model run.

 

 

 

## Value Changes

 

The Transport column does not change after time zero. The Distance column updates whenever the Transport object is used in any of the following activities: 

- Escort Person
- Transport Person
- Wait In Line
- Move Equipment

 

 

 

## Warmup Behavior

 

At the warmup time, this table resets the Distance column to zero.
