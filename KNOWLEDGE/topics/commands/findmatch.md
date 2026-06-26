---
id: findmatch
name: "findmatch"
kind: command
module: ""
signature: "findmatch(num nr, num matchexpression[, returnexpression = count, num reverseorder = 0])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: findmatch."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# findmatch

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
findmatch(num nr, num matchexpression[, returnexpression = count, num reverseorder = 0])
```

Performs a matching test a defined number of times, and when a match is found, returns a user-definable value
associated with that match. Within the match expression and return expression, "count" can be used to determine
which iteration your are on in the check. This can be used to search in a list of things for a matching item or criterion. If returnexpression is not included in the call, then findmatch() will return the count associated
with the matching expression. If no matches are found, findmatch() will return 0. If reverseorder is 1, then
it will go backwards, assigning count to nr first, then nr - 1, etc. down to 1. If
returnexpression is left out, then findmatch will return the count associated with the
found match.

## Example

```flexscript
int portNr = findmatch(current.outObjects.length, current.outObjects[count].subnodes.length < 5);
This example will return the output port number that corresponds to the first object it finds connected to
current's output ports that has a content less than 5.

treenode object = findmatch(current.outObjects.length, current.outObjects[count].subnodes.length < 5, current.outObjects[count]);
This example is similar to the previous example except it will return the object itself instead of the
associated port number.

int item2Rank = findmatch(current.subnodes.length, current.subnodes[count].type == 2);
This example will return the rank number of the first item found in current's queue that has an item type
of 2.
```

