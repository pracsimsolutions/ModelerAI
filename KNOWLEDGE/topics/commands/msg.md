---
id: msg
name: "msg"
kind: command
module: ""
signature: "msg(str caption, str text [, num buttontype]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Display a message dialog"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# msg

```flexscript
msg(str caption, str text [, num buttontype]) -> num
```

Display a message dialog

This command creates a message box with a title bar displaying the caption and the text written inside the box.
The buttons displayed on the message box are based on the value for buttontype.
If OK or Yes are pressed, msg() returns 1. If No is pressed, msg() returns 0. The value returned when Cancel is pressed depends on the buttontype.
If buttontype is 3, Cancel returns -1; otherwise it returns 0.
If buttontype is not specified, it is assumed to be 0.

Values for buttontype: 0 - OK, Cancel 1 - OK 2 - OK, Cancel 3 - Yes, No, Cancel 4 - Yes, No

## Example

```flexscript
if (msg("Finished", "Close the database?") == 1) dbclose();
```

