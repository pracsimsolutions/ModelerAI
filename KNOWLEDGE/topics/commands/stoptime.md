---
id: stoptime
name: "stoptime"
kind: command
module: ""
signature: "stoptime(num time [, num stopNum=1]) -> num"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Sets the time when the simulation should stop"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stoptime

```flexscript
stoptime(num time [, num stopNum=1]) -> num
```

Sets the time when the simulation should stop

This command sets the time at which the simulation should stop running. The model will not necessarily stop at the exact time specified, but will stop at the first system check on or after the specified stop time. The model may stop early if there are no more events to execute. If stopNum is specified, the stop time at that rank will be set. If time is -1, the command returns the stoptime of stopNum. If time is 0 and stopNum is -1, all stop times will be reset such that there is only one disabled stop time at time 0.

## Example

```flexscript
if (stoptime(-1) < 500) stoptime(500);
This sets the stoptime to 500 if it was less than 500 to begin with.
```

