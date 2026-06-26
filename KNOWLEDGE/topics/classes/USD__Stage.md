---
id: usd__stage
name: "USD::Stage"
kind: class
module: "USD"
signature: "USD::Stage"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents a USD Stage."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.Stage.xml"
---

# USD::Stage

A class that represents a USD Stage.

## Methods

### definePrim

```flexscript
USD.Prim USD::Stage.definePrim(string primPath, string typeName = "")
```

Create a USD.Prim in the stage.

**Parameters:**
- `primPath` *(string)* — The path of the new prim.
- `typeName` *(string, default "")* — Optional. The type of prim being created.

**Returns:** The newly created USD.Prim.

Prim paths follow a filepath-structure. For example, if you want to create a prim named "Processor1" nested under a
prim called "World", your primPath would be the string "/World/Processor1".
Note: if the "World" prim didn't exist in this example, it would be created to fulfill the creation of the prim. If you try to define a
prim that already exists at the given path with no typeName or the same typeName, no new prim will be created. If the typeName is different,
it will redefine the type of the existing prim.
Prim types are schemas that are
generally found in the OpenUSD documentation or are user defined. The typeName parameter specifices what type of prim you want to create,
defaulting to no type. For example, you can create an "Xform"
(short for USD transformation) prim by running the code below.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.definePrim("/my/test/prim", "Xform");
if (prim) {
...
}

### findPrim

```flexscript
USD.Prim USD::Stage.findPrim(string primName)
```

Return a USD.Prim with a given name.

**Parameters:**
- `primName` *(string)* — The name of the prim to search for.

**Returns:** A USD.Prim.

Prim names follow a filepath-structure. For example, if you have a prim named "Processor1" nested under a
prim called "World", you would search for it with the string "/World/Processor1".

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.findPrim("/World/Processor1");
if (prim) {
...
}

### getPrims

```flexscript
Array USD::Stage.getPrims()
```

Return an Array of all USD.Prim objects on the stage.

**Returns:** An array of USD.Prim objects.

This function performs a Depth First Search on the stage to find all prims in the stage.

USD.Stage stage = Model.find("USD Stage1");
Array prims = stage.getPrims();
for (int i = 1; i <= prims.length; i++) {
...
}

### processLiveUpdates

```flexscript
USD::Stage.processLiveUpdates()
```

Send live updates to the server and process live updates received from the server.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.definePrim("/my/test/prim", "Xform");
stage.processLiveUpdates(); // send changes to the server; receive any changes and update the stage

### reload

```flexscript
int USD::Stage.reload()
```

Reload the stage.

**Returns:** Whether or not the reload was successful. Returns 1 for success; otherwise 0.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.definePrim("/my/test/prim", "Xform");
stage.reload(); // reverts changes to the stage

### save

```flexscript
Variant USD::Stage.save()
```

Save changes to the stage.

**Returns:** Whether or not the save was successful. Returns 1 for success; otherwise 0.

The .usd* file will be updated.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.definePrim("/my/test/prim", "Xform");
stage.save(); // saves the new prim to the stage

## Properties

### autoWriteAnimationOffsets

```flexscript
int USD::Stage.autoWriteAnimationOffsets
```

Allows users to toggle whether saving the USD Stage should automatically write offset times for animations.

Currently, when you save a USD Stage in FlexSim, it will automatically export offsets for object references. This
means that when you view the USD Stage in another program, objects will appear in the state of the animation that FlexSim shows them
in when you saved.
This behavior is on (1) by default. To turn it off, set this value to 0. If turned off, you can set your own offsets by using the
USD.Reference class to build a reference and set it on a USD.Prim's "references" property.

### defaultPrim

```flexscript
USD.Prim USD::Stage.defaultPrim (readonly)
```

The default prim set for the stage.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;

### endTime

```flexscript
double USD::Stage.endTime
```

The end time of the stage's animation.

### startTime

```flexscript
double USD::Stage.startTime
```

The start time of the stage's animation.

