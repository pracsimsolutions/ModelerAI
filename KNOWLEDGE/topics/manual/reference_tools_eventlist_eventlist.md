---
id: reference_tools_eventlist_eventlist
name: "Event List"
kind: manual
breadcrumb: ["Reference","Tools","EventList","EventList"]
tags: ["manual","reference","tools","eventlist"]
source: "manual/Reference/Tools/EventList/EventList.html"
---

# Event List

## Overview and Key Concepts

 

The Event List shows all the pending events for the model. It is useful for seeing when different events will occur in order to debug modeling issues. If you have a problem that happens during a particular event, the Event List is useful for seeing information about that event to help track down the source of the problem.

 

 

You can access the Event List from the Debug menu on the main toolbar. If you want to only view the events for a particular object, you can right-click on the 3D object, point to View, then select View Object Events.

 

 

 

## Properties

 

The Event List has the following properties:

 

### Time

 

This is the time that the event occurs. The time value is displayed using the model's precision.

  

#### Fast Forwarding to an Event Time

 

You can fast forward the simulation to a specific event's time. Click in the time cell of the desired event, and press the arrow button that appears on the right side of that cell.

  

### Object

 

This is the path to the object, relative to the model, that the event affects.

 

### Event Type

 

This is the type of event. It is the event code and will show a number value for event codes without registered names. You can use the "seteventlistlegendentry" application command to register a name for custom event types.

 

For example: `applicationcommand("seteventlistlegendentry", 102, "My Event Type", 0);` will set event code 102 to show "My Event Type" as its name in the list.

 

### Involved

 

This is the path to the involved object for the event.

 

### Event Data

 

This value's use depends on the event and may not be used for all event types.

 

### Filters

 

The Event List can be filtered based on any column. Columns with an active filter will display a . To add/edit a filter, hover over the column header and click the button.

 

### Time

 

Each time filter has a begin (left) and an end (right) field. Only events that occurred within those two times will be displayed. If multiple time filters are present, events that occurred within any time range will be shown. To add a filter, click the icon. To remove a filter, click the icon.

 

 

### Text

 

All other columns can be filtered by text values. If the the cell's value contains any of the filter values, that cell will be shown. To add a filter, click the icon. To remove a filter, click the icon.
