---
id: reference_peopleobjects_processflowactivities_basic_createperson_createperson
name: "Create Person"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","Basic","CreatePerson","CreatePerson"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/Basic/CreatePerson/CreatePerson.html"
---

# Create Person

## Overview and Key Concepts

 

The Create Person activity creates a Person flowitem.

 

This creates copies of a Person flowitem from the flowitem bin in the specified destination. The created person can optionally be attached to a Person Process Flow object.

 

 

 

 

## Connectors

 

The Create Person activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Create Person activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Person

 

The **Person** defines the flowitem in the Flowitem bin to be created.

 

### Assign To

 

Assigns a reference on the specified label/node to the created person. See the Assign To section of Common Properties for more information.

 

### Destination

 

The **Destination** defines the object where the created person will be placed.

 

### Label Set

 

The **Label Set** specifies which label set will be applied to the created object. You can pick a Label Set from the picklist. You can also specify a Label Set by its rank, or by its name. For more information on Label Sets, see the People Settings topic.

 

### Person Flow

 

If specified, the **Person Flow** defines the Person Process Flow that the object will be attached to after being created.

 

### Copy Token Labels

 

If checked, all the labels currently on the token will be copied to the created person.
