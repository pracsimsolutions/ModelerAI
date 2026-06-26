---
id: reference_tools_userevents_userevents
name: "User Events"
kind: manual
breadcrumb: ["Reference","Tools","UserEvents","UserEvents"]
tags: ["manual","reference","tools","userevents"]
source: "manual/Reference/Tools/UserEvents/UserEvents.html"
---

# User Events

## Overview and Key Concepts

 

User Events are FlexScript functions that execute at set times during the model's run, but are not connected with any specific, visible object. They are created and stored in model's */Tools/UserEvents* folder. A model can have any number of user events.

 

 

User Events are accessed from the Toolbox.

 

 

 

## Properties

 

User Events have the following properties:

 

| Icon | Description |
| --- | --- |
|  | Adds a new blank User Event. |
|  | Removes the selected User Event. |
|  | Reorders User Events Up or Down in the list. |
|  | Adds the User Event to a User Library as either a Draggable Icon or an Auto-Install Component. |

 

### User Events List

 

Displays all of the Model's User Events. Changing from one User Event to another will apply any changes made to the previous User Event before displaying the newly selected User Event.

 

### Name

 

This is the name of the user event. This is purely for the modeler's convenience and has no effect on the model. It is helpful to be descriptive of what the user event does.

 

### Execute event on reset only

 

If this box is checked the event will only be executed when the reset button is pressed.

 

### First Event Time

 

This is the time in model units that the User Event will occur.

 

### Repeat Event

 

If this box is checked, as soon as the user event executes, it begins counting towards the next execution time as defined by the Repeat Event Time.

 

### Repeat Event Time

 

If the Repeat Event box is checked, this field will be enabled. Once the first User Event executes, the User Event will repeat in regular intervals defined by this time (in model units).

 

### Event Code

 

This is where the FlexScript code for the event is written. Any valid FlexScript statements can be used in this picklist.
