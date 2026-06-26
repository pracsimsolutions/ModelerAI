---
id: createevent
name: "createevent"
kind: command
module: ""
signature: "createevent(obj object, num time, num code[, str data, node involved]) -> num"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: "listed"
description: "FlexScript command: createevent."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createevent

```flexscript
createevent(obj object, num time, num code[, str data, node involved]) -> num
```

For developer use. This command creates an event that will occur sometime in the future in the model's run. The event will be created for the object specified by object. It will occur time seconds from the time that this command is called. If an event needs to occur that the same simulation time that it was created at, time should be 0. When the event occurs, the OnTimerEvent event function of the class of object will be called. This function will be passed code, data, and involved. These values can be accessed in OnTimerEvent as eventcode, eventdata, and i. Generally, eventcode is used to determine what type of event is being created or executed. Eventdata is used as a description of the event for the user's benefit. It is generally not important to the execution of OnTimerEvent. Any objects that are involved in the event, such as a flowitem that is entering or exiting, are passed as involved and are referenced in OnTimerEvent as i. There are predefined values for code that the user should use listed below:

1 - EVENT_OPENINPUTS 2 - EVENT_OPENOUTPUTS 3 - EVENT_PROCESSFINISH 4 - EVENT_CREATEPART 5 - EVENT_DESTROYPRODUCT 6 - EVENT_CONVEYEND 7 - EVENT_BEGINLOADTIME 8 - EVENT_BEGINUNLOADTIME 9 - EVENT_ENDLOADTIME 10 - EVENT_ENDUNLOADTIME 11 - EVENT_ENDDELAYTIME 12 - EVENT_BEGINDOWNTIME 13 - EVENT_ENDDOWNTIME 14 - EVENT_ENDWAITTIME 15 - EVENT_MEETBATCHQTY 16 - EVENT_ENDTRAVELTIME 17 - EVENT_ENDSETUP
If a user wishes to create their own types of events, they should use values for code that are greater than 100. See also eventcode and eventdata.

## Example

```flexscript
if (current.subnodes.length >= current.mincontent) { createevent(current,5,EVENT_OPENOUTPUTS,"openoutputs",involved); }
This creates an event that will occur in 5 simulation seconds that will open the output ports of the object specified as current, for the flowitem specified by involved, if the content of current is less than the label called mincontent of the object referenced by current.
```

