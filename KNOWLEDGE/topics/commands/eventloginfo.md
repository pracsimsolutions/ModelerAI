---
id: eventloginfo
name: "eventloginfo"
kind: command
module: ""
signature: "eventloginfo(num operation, num n1) -> str"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: eventloginfo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# eventloginfo

```flexscript
eventloginfo(num operation, num n1) -> str
```

For developer use. Returns a string value describing something related to the Event Log.

Operations:
ELI_GET_OBJECTPATH 1 - returns the path of the object at index n1 from the objectpaths array.
ELI_GET_INVOLVEDPATH 2 - returns the path of the object at index n1 from the involvedpaths array.
ELI_GET_OTHERPATH 3 - returns the path of the object at index n1 from the otherpaths array.
ELI_EVENTTYPE_NAME 4 - returns the name of the eventtype at index n1 from the eventtypes array.
ELI_LOGGEDEVENTTYPE_NAME 5 - returns the name of the logged eventtype at index n1 from the loggedeventtypes array.

Other operations relating to the Event Log that return number values can be accessed using the applicationcommand() function:
"seteventlogging" - enables or disables logging for eventtype n1.
"geteventlogging" - returns whether logging is enabled for eventtype n1.
"getobjectpathslistsize" - returns the size of the objectpaths array.
"getinvolvedpathslistsize" - returns the size of the involvedpaths array.
"getotherpathslistsize" - returns the size of the otherpaths array.
"geteventtypeslistsize" - returns the size of the eventtypes array.
"getloggedeventtypeslistsize" - returns the size of the loggedeventtypes array.
"geteventtypecode" - returns the index of the eventtype by name passed as n1.

## Example

```flexscript
string objectpath = eventloginfo(ELI_GET_OBJECTPATH,1);
```

