---
id: debug
name: "debug"
kind: command
module: ""
signature: "debug([num operation, num/obj p2, num/obj p3])"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "Acts like a breakpoint in Flexscript to start debugging"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# debug

```flexscript
debug([num operation, num/obj p2, num/obj p3])
```

Acts like a breakpoint in Flexscript to start debugging

For developer use. This command is used to control the Flexscript step debugger. Calling this command without any parameters will behave as if there is a breakpoint at that line of code.

Operations:
DEBUG_STOP 0
DEBUG_STEP 1
DEBUG_CONTINUE 2
DEBUG_IS_STOPPED 3
DEBUG_GET_LINE_NR 4
DEBUG_DUMP_VARIABLES 5
DEBUG_STOP_SIMULATION 6
DEBUG_REFRESH_BREAK_POINTS 7
DEBUG_ADD_BREAK_POINT 8
DEBUG_DELETE_BREAK_POINT 9
DEBUG_GET_WATCH_VARIABLE 10
DEBUG_ENABLE_CODE_PROFILING 11
DEBUG_PRINT_CODE_PROFILING_INFO 12
DEBUG_RESET_CODE_PROFILING 13

## Example

```flexscript
debug();
```

