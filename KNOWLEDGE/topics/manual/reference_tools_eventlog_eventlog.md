---
id: reference_tools_eventlog_eventlog
name: "Event Log"
kind: manual
breadcrumb: ["Reference","Tools","EventLog","EventLog"]
tags: ["manual","reference","tools","eventlog"]
source: "manual/Reference/Tools/EventLog/EventLog.html"
---

# Event Log

## Overview and Key Concepts

 

When Enable Logging is checked, the Event Log will create a record of events that occur in the model. It is useful for seeing the order in which certain events took place. For each event that happens in the model, multiple entries may be made in the Event Log to explain what happened during that event. These multiple entries will all have the same time and all be logged simultaneously when you press the Step button. The event log will be cleared when the model is reset.

 

 

Some exceptions will be recorded in the event log. The entry immediately preceeding the exception entry will give you a clue as to where the code is that caused the exception to happen. This is particularly useful if the exception was caused by improper code in an object's trigger. Any exceptions reported in the System Console will also be reported in the Event Log.

 

The Event Log is accessed from the Debug menu on the main toolbar.

 

 

 

## Enable Logging from the Command Line

 

In some cases, such as when FlexSim runs in the background, it can be useful to create an event log. You can enable an event log from the command line. This kind of log is sometimes called a **system log**. The system log is controled by the following switches:

 

| Switch | Value | Example |
| --- | --- | --- |
| /logevents | An absolute path to a target file. The directory must exist. If the file doesn't exist, a new file with the given name will be created. This switch enables the system log. The other event log switched require this switch to be present. | `/logevents "C:\Documents\FlexSim 2024 Projects\SystemLog.sqlite"` |
| /logname | The name of the event log within the given file. The default is "__system". | `/logname "MyEventLog"` |
| /loglevel | One of the following words: INFO, WARN, ERROR, or FATAL. Events lower than the specified level will not be logged. The default is INFO. | `/loglevel ERROR` |
| /eventcachesize | A positive integer. For performance reasons, events are not written to the file immediately. FlexSim caches events in memory until the given number is reached and then writes all cached events in one write operation. The default is 10000. The minimum is 1, meaning each event would be written to the file immediately. This is very slow, but useful if debugging a crash, as the event previous to the crash will be written to the file. | `/eventcachesize 100` |

  

#### The system log is separate from the model log.

 

The system log (specified on the command line) is separate from the log specified in the model. Each log has its own settings. It is possible to log events into either or both logs. The system log is not cleared when the model is reset.

   

#### Viewing a System Log

 

The Event Log object in the model has a variable specifying the event log name. By default, this value is set to "EventLog1". To view a system log, you can either change that variable or you can set the system log name to "EventLog1" with the `/logname` switch. Then you can open the system log from within FlexSim.

  

  

## Properties

 

The Event Log has the following properties:

 

### Enable Logging

 

This will enable or disable event logging. The model will run much slower when logging is enabled so you should disable logging when you are finished using the event log.

 

### Log File Path

 

The Event Log stores its data in a file on the hard drive. By default, the path shown is a relative path. The path is relative to the model's directory (`modeldir()`). For new models, the path is relative to the Documents directory (`documentsdir()`) until the model is saved. You can change the path using the the accompanying browse button . If you choose a destination outside the model directory, the absolute path to the destination is shown.

 

### Settings...

 

Within the settings window, you can set up which events should be recorded in the event log in three ways. First, you can filter by model time. You can adjust the start time and optionally the end time. Only events that occur between these times will be recorded. In addition, you can filter by event type. Events that have already been recorded will not be affected by changing these settings. Events that occur after changing these settings will only be recorded if they are enabled here. Finally, you can filter by level. Only events that have the same or higher level will be logged.

 

 The start and end time are explained in the following sections. 

### Start Time

 

If you only want to log a specific time period, you can enter a start time for when the logging will begin.

 

### End Time

 

Optionally, you can specify an end time for when you want the logging to stop. If the end time is less than or equal to the start time, it will be ignored.

 

### Export...

 

This will export the Event Log as a csv file. If will only export valid events, ignoring any events that have been filtered out.

 

### The Table

 

The table has the following properties:

 

- **Time** - This is the time that the event happened. The entries happened in order from top to bottom. Entries recorded with the same time happened in the order shown and may have happened during the same model event. #### Increasing Precision in the Time Column Event times are stored as 64-bit floating point values. To increase the precision in the view, you can change the **Decimal Precision** in the Model Settings window.
- **Event** - This is the type of event. You can enable or disable logging for certain event types in the Settings window.
- **Object** - This is the path to the event's object.
- **Involved** - This is the path to the involved object for the event.
- **P1 - P4** - These values depend on the event and may not be used for all event types. Usually they give you information about what parameters were passed into the event or more information about the event type. This is useful for debugging if parameter values are not what you expect them to be. If you use the EventLog API to log custom events, you can set these values to any arbitrary value.

 

### Filters

 

The Event Log can be filtered based on the Time, Event, Object and Involved columns. Columns with an active filter will display a . To add/edit a filter, left-click on the header name for the desired filter. Event log entries that are no longer displayed because they have been filtered will not be exported with the Export button.

 

It has the following properties:

 

- **Time** - Each time filter has a begin (left) and an end (right) field. Only events that occurred within those two times will be displayed. To add a filter, click the icon. To remove a filter, click the icon.
- **Level** - This list allows you to filter the list by the level of each event. To include or exclude a level from the list, check or uncheck the box next to its name.
- **Event** - This list allows you to filter the list by which event or trigger the event was associated with. To include or exclude an event from the list, check or uncheck the box next to its name.
- **Object** - This list allows you to filter the list by which object generated the event. To include or exclude an object in the list, check or uncheck the box next to its name.
- **Involved** - This list allows you to filter the list by which object is involved in the event. To include or exclude an object in the list, check or uncheck the box next to its name.
