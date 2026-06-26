---
id: reference_peopleobjects_processflowactivities_basic_removeperson_removeperson
name: "Remove Person"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","Basic","RemovePerson","RemovePerson"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/Basic/RemovePerson/RemovePerson.html"
---

# Remove Person

## Overview and Key Concepts

 

The Remove Person activity removes a person flowitem from the 3D simulation model.

 

 

 

 

## Connectors

 

The Remove Person activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Remove Person activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Person

 

The **Person** defines the person flowitem in the 3D simulation model to be removed.

 

### Remove Statistically Only

 

When checked, removes the person from the model statistically, but not physically. The person remains in the model, but is hidden and does not count towards the model census. Another Remove Person activity is needed to later remove the person from the model. This is useful when using a Person Flow and there are activities related to the person that happen after the person leaves the model. Checking this box allows the instance associated with the person to stay around while updating the statistics as if the person had left.
