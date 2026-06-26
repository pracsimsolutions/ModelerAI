---
id: agv__routingconstraint
name: "AGV::RoutingConstraint"
kind: class
module: "AGV"
signature: "AGV::RoutingConstraint"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "Enumerates possible routing constraint values used in AGV.AllocatableObject.routingConstraint"
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.RoutingConstraint.xml"
---

# AGV::RoutingConstraint

Enumerates possible routing constraint values used in AGV.AllocatableObject.routingConstraint

## Properties

### BackwardOnly

```flexscript
int AGV::RoutingConstraint.BackwardOnly (readonly)
```

An AGV may only travel through the object if it is traveling in the path's reverse/backward direction.

### Blocked

```flexscript
int AGV::RoutingConstraint.Blocked (readonly)
```

An AGV may not travel to, from, or through the object.

### ForwardOnly

```flexscript
int AGV::RoutingConstraint.ForwardOnly (readonly)
```

An AGV may only travel through the object if it is traveling in the path's forward direction.

### None

```flexscript
int AGV::RoutingConstraint.None (readonly)
```

No constraints on routing, meaning an AGV may travel through the object.

### PassThroughBlocked

```flexscript
int AGV::RoutingConstraint.PassThroughBlocked (readonly)
```

An AGV may travel to or from the control point as an origin or destination, but it may not travel 'through' the object.

