---
id: maintenance
name: "maintenance"
kind: command
module: ""
signature: "maintenance(num command[, num state, str strparam])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "parser"
description: "FlexScript command: maintenance."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# maintenance

```flexscript
maintenance(num command[, num state, str strparam])
```

For developer use. Set a maintenance switch.

The first parameter designates the operation. The second parameter designates a state to set it to.

Maintenance switches:
0. print this list
1. deadlinks
2. atttrbute bind method (0=names, 1=indices, 2=bootstrap)
3. numberprecision
4. display binding report
5. usercollisions
6. post-onsend
7. loadsavemode
8. disablefscheck
9. disablefspreprocess
10. splinetension
11. streamalizer
12. immediately load extended docs
13. save reps at experiment end
14. suppress object library compile
15. suppress global compile
16. set runtime info
17. get runtime info
18. bind c++ functions
19. autorebind c++ functions onoff
20. get presentation mode
21. refresh main menu
22. is optquest enabled
23. flypaths in seconds units
24. chached ports on/off
25. bind on reset on/off
26. set category name (user command group is 22, other 23, 24+ is available)
27. object sizermode 0. default 1. 3-axis
28. show hidden nodes in tree
29. hide 3d cursor
30. supress simclock redraw
31. use parser two
32. grid draw settings: bitwise field: 0x1-auto expand grid, 0x2-show grid axes, 0x4-show grid origin
1000. DDE

## Example

```flexscript
maintenance(0)
```

