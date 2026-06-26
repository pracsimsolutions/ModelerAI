---
id: pullitem
name: "pullitem"
kind: command
module: ""
signature: "pullitem(obj fixedresource, obj item, num portnr[, num bypassflags]) -> num"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "FlexScript command: pullitem."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pullitem

```flexscript
pullitem(obj fixedresource, obj item, num portnr[, num bypassflags]) -> num
```

Tells the fixedresource to evaluate its pull requirement for the passed item that is in the object through the input port portnr. If the pull evaluation is true, this command pulls the item immediately. This command returns 1 if the item was successfully pulled and 0 if not. If the item cannot be pulled for any of various reasons (such as closed ports or the item not being released), then this command returns 0. The bypassflags parameter is a bitwise combination of macros that start with BYPASS_ to specify certain criteria to ignore in determining item availability. You can specify BYPASS_ALL to immediately pull the item regardless of availability.

## Example

```flexscript
pullitem(current, item, 1); // See also Pull Best Item picklist option
```

