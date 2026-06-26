---
id: conveyor__mergecontroller
name: "Conveyor::MergeController"
kind: class
module: "Conveyor"
signature: "Conveyor::MergeController"
aliases: []
tags: ["class", "conveyor-module"]
deprecated: false
replacedBy: null
description: "Allows access to the lanes of a MergeController object"
seeAlso: []
source: "help\\FlexScriptAPIReference\\Conveyor\\Conveyor.MergeController.xml"
---

# Conveyor::MergeController

Allows access to the lanes of a MergeController object

Conveyor.MergeController mc = model().subnodes["MergeController1"];
var slugBuilder = mc.lanes[1];

## Properties

### lanes

```flexscript
Conveyor::MergeController.lanes (readonly)
```

Allows array-like access (by index) to the lanes of a MergeController as Conveyor.SlugBuilder objects.

