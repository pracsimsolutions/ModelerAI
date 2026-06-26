---
id: addtoeventtypelegend
name: "addtoeventtypelegend"
kind: command
module: ""
signature: "addtoeventtypelegend(str name, num parameter, num value, str legendname)"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: addtoeventtypelegend."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addtoeventtypelegend

```flexscript
addtoeventtypelegend(str name, num parameter, num value, str legendname)
```

The eventtype legend allows you to associate a string with a given value for a given parameter in that eventtype.
Passing a negative number for 'parameter' allows you to add a preceeding string to explain the parameter.
If the 'parameter' is negative and the 'value' is -1, then it assumes the number is a pointer to a node and will display the node's path as a string.

## Example

```flexscript
addtoeventtypelegend(index,-1,-1,"resource: "); // for p1, display the text "resource: " and display the path for the object
addtoeventtypelegend(index,-2,0,"covermode: "); // for p2, display the text "covermode: "
addtoeventtypelegend(index,2,1,"YellowToGreen"); // for p2, a value of 1 should display "YellowToGreen" instead of the number 1
addtoeventtypelegend(index,2,2,"RedToGreen"); // for p2, a value of 2 should display "RedToGreen" instead of the number 2
addtoeventtypelegend(index,3,0,""); // for p3, this makes zeroes blank
addtoeventtypelegend(index,4,0,""); // for p4, this makes zeroes blank
```

