---
id: eventdata
name: "eventdata"
kind: command
module: ""
signature: "eventdata<no parentheses> -> str/num/node"
aliases: []
tags: ["command", "group-events"]
deprecated: false
replacedBy: null
description: "FlexScript command: eventdata."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# eventdata

```flexscript
eventdata<no parentheses> -> str/num/node
```

For developer use. This command returns the string value that was passed to the createevent() command as data. It is only used in the OnTimerEvent function of the library objects. Generally, it is used as a description to the user of the type of event that is occurring. It is not generally used in the execution of the OnTimerEvent function, for speed reasons. This command should not have parentheses when it is called. See also createevent() and eventcode.

## Example

```flexscript
if (comparetext(eventdata,"openinputs")) { thisobjectdata->openinputs(); }
This calls a method called openinputs on the object referenced by thisobjectdata if the string value passed to createevent() is equal to "openinputs".
```

