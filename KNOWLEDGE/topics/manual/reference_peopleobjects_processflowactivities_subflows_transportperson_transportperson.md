---
id: reference_peopleobjects_processflowactivities_subflows_transportperson_transportperson
name: "Transport Person"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","SubFlows","TransportPerson","TransportPerson"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/SubFlows/TransportPerson/TransportPerson.html"
---

# Transport Person

## Overview and Key Concepts

 

The Transport Person activity will cause a staff member to transport a person to a destination using a transport.

 

 

The staff will exit their current location, walk to the transport, pick it up, and then walk to the person being transported. The person will exit their current location and enter the transport. The staff will then take the transport to the destination. Upon arriving at the destination, the person will exit the transport unless the Keep Person on Transport checkbox is checked. If the destination is an object, the person will enter the destination while the staff will drop off the transport and also enter the destination.

 

  

#### Sub Flow Activity

 

The Transport Person activity is a Sub Flow Activity and its Inner Flow can be viewed if you wish to learn more about how it works, debug its behavior, or copy its logic.

  

 

 

## Connectors

 

The Transport Person activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Transport Person activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Person

 

The **Person** defines the person who will be transported. If an array of people is specified, the first will be transported while the rest follow along.

 

### Destination

 

The **Destination** defines the object, or position, the person will be transported to.

 

### Staff

 

The **Staff** defines the staff who will be transporting the person. If an array of staff is specified, the first will push the transport while the rest follow along.

 

### Transport

 

The **Transport** defines the transport which will be used to transport the person.

 

### Keep Person on Transport

 

The **Keep Person on Transport** checkbox defines what the person does when the transport reaches the destination. By default the box is not checked and the person will exit the transport and then enter the destination if it is an object. If checked, the person will not exit the transport and if the destination is a location the transport (with the person still inside) will be moved inside the location.
