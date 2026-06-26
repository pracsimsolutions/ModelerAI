---
id: eventcode
name: "eventcode"
kind: command
module: ""
signature: "eventcode<no parentheses> -> num"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: eventcode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# eventcode

```flexscript
eventcode<no parentheses> -> num
```

For developer use. This command returns the number value that was passed to the createevent() command as code. It is only used in the OnTimerEvent function of the library objects. It is generally used to identify the type of event that is occurring. This command should not have parentheses when it is called. See also createevent() and eventdata.

## Example

```flexscript
if (eventcode == EVENT_OPENINPUTS) thisobjectdata->openinputs();
This calls a method called openinputs() on the object referenced by thisobjectdata if the value EVENT_OPENINPUTS was passed to the createevent() function.
```

