---
id: setmessageloopsleep
name: "setmessageloopsleep"
kind: command
module: ""
signature: "setmessageloopsleep(num interval, num sleeptime)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "carefully"
description: "FlexScript command: setmessageloopsleep."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setmessageloopsleep

```flexscript
setmessageloopsleep(num interval, num sleeptime)
```

For developer use. Set a sleep time for each cycle of the message loop to decrease cpu load. Use carefully because a large value can make FlexSim unresponsive to user input. Every interval milliseconds, FlexSim will pause for a period of sleeptime milliseconds. This command may help certain laptops that tend to overheat due to excessive cpu usage.

## Example

```flexscript
setmessageloopsleep(1)
```

