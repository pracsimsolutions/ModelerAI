---
id: setcursor
name: "setcursor"
kind: command
module: ""
signature: "setcursor(num cursorid[, num forgetCount = 0])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Sets the visual look of the mouse pointer"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setcursor

```flexscript
setcursor(num cursorid[, num forgetCount = 0])
```

Sets the visual look of the mouse pointer

Sets the visual look of the mouse pointer. Valid ids are as follows

1: Arrow
2: IBeam
3: Wait
4: Cross
5: UpArrow
6: Size
7: Icon
8: SizeNWSE
9: SizeNESW
10: SizeWE
11: SizeNS
12: SizeAll
13: No
14: Hand
15: AppStarting
16: Help
The forgetCount defines if/when FlexSim should go back to the default cursor. If 0 (default),
then FlexSim will keep that cursor until the user calls setcursor() again. If you pass a
positive number then FlexSim will reset the cursor back to the default after forgetCount number
of mouse move messages are received from the system, if you have not called setcursor() again
in that interval.

## Example

```flexscript
setcursor(3)
```

