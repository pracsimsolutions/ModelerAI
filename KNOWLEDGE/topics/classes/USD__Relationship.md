---
id: usd__relationship
name: "USD::Relationship"
kind: class
module: "USD"
signature: "USD::Relationship"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents a USD Relationship."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.Relationship.xml"
---

# USD::Relationship

A class that represents a USD Relationship.

## Methods

### getTargets

```flexscript
Array USD::Relationship.getTargets()
```

Get an array of targets as strings.

**Returns:** An array of targets as strings.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
USD.Relationship relationship = prim.getProperty("targets");
if (relationship) {
Array targets = relationship.getTargets();
}

### setTargets

```flexscript
void USD::Relationship.setTargets()
```

Set the array of targets for the USD.Relationship.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
Array targets = ["/World/Processor1"];
USD.Relationship relationship = prim.createRelationship("myTargets");
if (relationship) {
relationship.setTargets(targets);
}

