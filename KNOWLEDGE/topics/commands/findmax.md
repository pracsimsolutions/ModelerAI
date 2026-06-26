---
id: findmax
name: "findmax"
kind: command
module: ""
signature: "findmax(num nr, num valueexpression[, returnexpression = maxvalue, num criterion = true, num reverseorder = 0])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: findmax."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# findmax

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
findmax(num nr, num valueexpression[, returnexpression = maxvalue, num criterion = true, num reverseorder = 0])
```

Iteratively evaluates an expression, and then returns the maximum value found, or a user-definable return value
associated with that maximum value. Within the value expression, return expression and criterion, "count" can be used to determine
which iteration you are on in the check. If the return expression is not defined, the maximum value found will be returned.
Additionally, you can specify a criterion to filter out values as invalid. If criterion is defined and nothing meets the
criterion, 0 will be returned.

## Example

```flexscript
double latestCreation = findmax(current.subnodes.length, getcreationtime(current.subnodes[count]));
This example will return the maximum creation time of all items in current.

int largestQueuePortNr = findmax(current.inObjects.length, current.inObjects[count].subnodes.length;
This example will return the input port number associated with the object with the largest queue of items.

int largestQueuePortNr = findmax(current.inObjects.length, current.inObjects[count].subnodes.length, count, getitemstate(current.inObjects[count].first) == FR_STATE_READY);
This example is the same as the previous example, but additionally in includes a criterion that the first item in the upstream
object must also have been released.
```

