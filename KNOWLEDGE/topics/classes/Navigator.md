---
id: navigator
name: "Navigator"
kind: class
module: ""
signature: "Navigator"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a TaskExecuter's navigator. In other words, the object that manages travel tasks for the task executer."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TaskExecuter\\Navigator.xml"
---

# Navigator

A class that represents a TaskExecuter's navigator. In other words, the object that manages travel tasks for the task executer.

## Methods

### getCost

```flexscript
double Navigator.getCost(Variant origin, Variant destination, Variant info)
```

A generic method that gets route costing information associated with a TaskExecuter traveling from some origin to some destination.

**Parameters:**
- `origin` *(Variant)* — Navigator-dependent parameter defining the origin. This can be the following: Default NavigatorTask executer reference. Node Network NavigatorTask executer reference Network node reference Reference to an object connected to a network node AGV NavigatorTask executer reference Reference to a control-point-connected object AGV path reference Control point reference An array with up to three values: Task executer, path, control point or control-point-connected object An offset value representing an offset distance from that object on its associated path Boolean value defining whether or not a given AGV is oriented 'backward' on the path. 1 means backward, 0 or absent means forward. Note that the cost may be different for forward vs. backward oriented AGVs when the path system has orientation constraints defined, or if cost is speed-based and backward speed is different than forward speed. . A* NavigatorTask executer referenceObject reference An array of three numbers representing a x/y/z model location GIS NavigatorTask executer referenceGIS point reference Reference to an object connected to a GIS point
- `destination` *(Variant)* — Navigator-dependent parameter defining the destination. This can generally take the same values as the origin parameter, with the following exceptions: It should not be a reference to a TaskExecuter.For AGVs, a third array value (AGV orientation) is not used.
- `info` *(Variant)* — Navigator-dependent parameter to modify how the cost is calculated, as described below: AGV NavigatorIf this parameter is a reference to a TaskExecuter, it will assume that agv-dependent cost calculations are to be associated with that object traveling in its current state, most importantly its current load type. This is useful in calculating speed-based costs when speed is dependent on load type. GIS NavigatorA value of 1 will return distance, a value of 2 will return duration, and a value of 3 will return general cost.

The implementation of this method is polymorphic as it depends somewhat on
which navigator it is being called on. For example, a default navigator's getCost() will
only work if you pass
a TaskExecuter as the origin, and some other object as a destination, and it will return
the same result as if you were to call distancetotravel(). On the other hand, a network
navigator can take a network node as an origin and a network node as a destination. In this
case, the result is the same as calling getnetworkdistance().

