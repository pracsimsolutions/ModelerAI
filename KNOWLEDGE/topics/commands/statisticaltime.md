---
id: statisticaltime
name: "statisticaltime"
kind: command
module: ""
signature: "statisticaltime() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Model.statisticalTime"
description: "Deprecated, use the Model class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# statisticaltime

> ⚠ **DEPRECATED.** Use `Model.statisticalTime` instead.

```flexscript
statisticaltime() -> num
```

Deprecated, use the Model class

This command is deprecated. Use Model.statisticalTime instead.

Returns the current simulation time with respect to when stats were last reset, such as since the warmup time has passed in an experiment or since resetstats() was executed.

## Example

```flexscript
item.TimeIn = statisticaltime();
This sets the label called "TimeIn" on the object referenced by item to the current simulation clock time.
```

