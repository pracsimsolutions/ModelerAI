---
id: evaluatepullcriteria
name: "evaluatepullcriteria"
kind: command
module: ""
signature: "evaluatepullcriteria(obj fixedresource, obj item, num portnr[, num bypassflags]) -> num"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Causes the FixedResource object to re-fire its pull requirement"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# evaluatepullcriteria

```flexscript
evaluatepullcriteria(obj fixedresource, obj item, num portnr[, num bypassflags]) -> num
```

Causes the FixedResource object to re-fire its pull requirement

Tells the FixedResource to evaluate its pull requirement for the passed item that is in the object through the input port portnr. This command returns the value returned by the pull requirement field if the item can be pulled. If the item cannot be pulled for any of various reasons (such as closed ports or the item not being released), then this command returns 0. The bypassflags parameter is a bitwise combination of macros that start with BYPASS_ to specify certain criteria to ignore in determining item availability.

## Example

```flexscript
evaluatepullcriteria(current, item, 1); // See also Pull Best Item picklist option
```

