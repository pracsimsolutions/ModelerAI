---
id: agv__bodyoffset
name: "AGV::BodyOffset"
kind: class
module: "AGV"
signature: "AGV::BodyOffset"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "Enumerates possible AGV body offset values"
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.BodyOffset.xml"
---

# AGV::BodyOffset

Enumerates possible AGV body offset values

The AGV.BodyOffset enumeration can be used to define target AGV body offsets for
customized
AGV travel tasks.
Each enumeration combines two facets:

Whether the offset corresponds the entire AGV "train" or to an individual "trailer"
The corresponding "edge" of the target object.

In defining the "edge" the following definitions apply:

Head Edge - the head edge of the object is the forward-most point along its x axis, regardless of travel direction.
Tail Edge - the tail edge of the object is the backward-most point along its x axis, regardless of travel direction.
Leading Edge - the leading edge of the object is the forward-most point in the AGV's travel direction.
When the AGV is traveling forward, its leading edge is the same as its head edge. When traveling backward its leading edge is the
same as its tail edge.
Trailing Edge - the trailing edge of the object is the backward-most point in the AGV's travel direction.
When the AGV is traveling forward, its trailing edge is the same as its tail edge. When traveling backward its trailing edge is
the same as its head edge.

## Properties

### TrailerHeadEdge

```flexscript
int AGV::BodyOffset.TrailerHeadEdge (readonly)
```

An AGV trailer's head edge.

### TrailerLeadingEdge

```flexscript
int AGV::BodyOffset.TrailerLeadingEdge (readonly)
```

An AGV trailer's leading edge.

### TrailerTailEdge

```flexscript
int AGV::BodyOffset.TrailerTailEdge (readonly)
```

An AGV trailer's tail edge.

### TrailerTrailingEdge

```flexscript
int AGV::BodyOffset.TrailerTrailingEdge (readonly)
```

An AGV trailer's trailing edge.

### TrainHeadEdge

```flexscript
int AGV::BodyOffset.TrainHeadEdge (readonly)
```

The AGV train's head edge.

### TrainLeadingEdge

```flexscript
int AGV::BodyOffset.TrainLeadingEdge (readonly)
```

The AGV train's leading edge.

### TrainTailEdge

```flexscript
int AGV::BodyOffset.TrainTailEdge (readonly)
```

The AGV train's tail edge.

### TrainTrailingEdge

```flexscript
int AGV::BodyOffset.TrainTrailingEdge (readonly)
```

The AGV train's trailing edge.

