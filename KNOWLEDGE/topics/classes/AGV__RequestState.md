---
id: agv__requeststate
name: "AGV::RequestState"
kind: class
module: "AGV"
signature: "AGV::RequestState"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "An enumeration of possible values for the AGV.AllocationPoint.requestState property."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.RequestState.xml"
---

# AGV::RequestState

An enumeration of possible values for the AGV.AllocationPoint.requestState property.

## Properties

### Allocated

```flexscript
int AGV::RequestState.Allocated (readonly)
```

The AGV has allocated the object.

### Requested

```flexscript
int AGV::RequestState.Requested (readonly)
```

The AGV has requested to allocate the object, but has not yet allocated it.

### Unrequested

```flexscript
int AGV::RequestState.Unrequested (readonly)
```

The AGV has not yet reached the point where it needs to request the allocation.

