---
id: conveyor__decisionpoint
name: "Conveyor::DecisionPoint"
kind: class
module: "Conveyor"
signature: "Conveyor::DecisionPoint"
aliases: []
tags: ["class", "conveyor-module"]
deprecated: false
replacedBy: null
description: "A class that provides properties for monitoring a conveyor decision point, photo eye, or station."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Conveyor\\Conveyor.DecisionPoint.xml"
---

# Conveyor::DecisionPoint

A class that provides properties for monitoring a conveyor decision point, photo eye, or station.

## Properties

### activeItem

```flexscript
Object Conveyor::DecisionPoint.activeItem (readonly)
```

Gets the item that is currently "active" associated with the decision point.

For a photo eye, this is the item that is covering the photo eye. For a decision point, it is the item
that has arrived but not yet continued from the decision point. For a station, it is the item that is
currently being processed, or being considered for processing, at the station.

### conveyor

```flexscript
Conveyor Conveyor::DecisionPoint.conveyor (readonly)
```

Gets the conveyor that the decision point is currently attached to.

