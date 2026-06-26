---
id: reference_peopleobjects_peopletables_peopletableconcepts_peopletableconcepts
name: "People Statistics Tables"
kind: manual
breadcrumb: ["Reference","PeopleObjects","PeopleTables","PeopleTableConcepts","PeopleTableConcepts"]
tags: ["manual","reference","peopleobjects","peopletables"]
source: "manual/Reference/PeopleObjects/PeopleTables/PeopleTableConcepts/PeopleTableConcepts.html"
---

# People Statistics Tables

## Overview and Key Concepts

 

The People Statistics Tables are tables that are added to every model that uses activities or objects from the People module. They record data about the following objects:

 

- Flowitems created with the Create Person activity (usually a Person)
- Staff
- Locations (including Multi-Locations)
- Transports
- Equipment
- Waiting Lines

 

Meaningful statistics can be derived from each table individually. For example, the StaffTravelDistance activity records how far each Staff object travels during the model run. You could use this table to derive the average distance travelled by all staff.

 

In addition, these tables are designed to work as a database, so that you can get more detailed statistics. For example, the StaffGroups table records which Staff objects belong in each Staff Group. By combining the information in this table with the information in the StaffTravelDistance table, you could determine the average distanced travelled by the staff in a certain group.

 

The best way to visualize data from these tables is using a People Template. More information on these templates can be found in the People Templates topic.

 

Some of these tables will grow longer as the model runs. Since not all users may be interested in all metrics, you can disable any of the People Statistics Tables, which will prevent it from collecting any data.

  

#### History Tables

 

If a table has the word "History" in its name, then that table is likely to grow longer and longer as the model runs.

  

All People Statistics Tables have an internal Statistics Collector. You can duplicate this internal collector, either to learn how the data for that table is collected, or to create a modified version of the table.

  

#### Time and Distance Units

 

All People Statistics Tables record durations in seconds and distances in meters, regardless of model units. All time stamps are recorded as DateTime values, so that the difference between two time stamps is always in seconds.

  

 

 

 

## Person Tables

 

The following tables record information about flowitems created with the Create Person activity:

 

- Person Labels
- Person States
- Person Milestones
- Person Travel Distance
- Person Wait Times
- Census History

 

 

 

## Staff Tables

 

The following tables record information about Staff objects in the Staff Group:

 

- Staff Groups
- Staff States
- Staff State History
- Staff Travel Distance
- Person Wait for Staff History

 

 

 

## Location Tables

 

The following tables record information about Location object in the Locations Group:

 

- Location Groups
- Location States
- Location State History
- Location Census History
- Location Staytime History
- Person Wait for Location

 

 

 

## Equipment Tables

 

The following tables record information about Equipment in the Equipment Group:

 

- Equipment Groups
- Equipment States
- Equipment State History
- Equipment Travel Distance
- Person Wait for Equipment History

 

 

 

## Transport Tables

 

The following tables record information about Transports in the Transport Group:

 

- Transport Groups
- Transport States
- Transport State History
- Transport Travel Distance
- Person Wait for Transport History

 

 

 

## Waiting Line Tables

 

The following tables record information about Waiting Line objects:

 

- Line Census History
- Line Staytime History

 

 

 

## Properties

 

Each People Statistics Table has the following properties:

 

 

- **Enabled** - Clear this box to disable this table. Note that People Templates that require this table will no longer function if the table is disabled.
- **Data Table** - Displays the data recorded by this table.
- **Copy internal Statistics Collector** - Click this button to have a copy of the internal statistics collector created and added to the toolbox.
- **Store data on hard drive** - Check this box to indicate that the data recorded by this table should be written to the hard drive to perserve RAM.
