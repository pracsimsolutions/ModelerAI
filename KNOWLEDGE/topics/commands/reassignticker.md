---
id: reassignticker
name: "reassignticker"
kind: command
module: ""
signature: "reassignticker(obj fluidobject, obj ticker[, int resortticker])"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Reassigns the Fluid Object's ticker"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# reassignticker

```flexscript
reassignticker(obj fluidobject, obj ticker[, int resortticker])
```

Reassigns the Fluid Object's ticker

Changes the Fluid Object passed in as the first parameter from the control of its current Ticker, to the Ticker that is passed in as the second parameter. The object will then have its fluid movement calculated whenever the new Ticker's tick-time elapses. It is recommended that Fluid Objects that are connected to each other always be controlled by the same Ticker. If resortticker is 1, then the new Ticker will immediately resort its list of controlled objects. This may change the order that some fluid objects are evaluated and may have side-effects in the model behavior.

## Example

```flexscript
reassignticker(current,Model.find("Ticker2",1);
```

