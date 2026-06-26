---
id: agv__trailer
name: "AGV::Trailer"
kind: class
module: "AGV"
signature: "AGV::Trailer"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related a trailer attached to an AGV."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.Trailer.xml"
---

# AGV::Trailer

Provides access to data and methods related a trailer attached to an AGV.

## Properties

### driver

```flexscript
AGV AGV::Trailer.driver (readonly)
```

Gets the AGV that this trailer is attached to.

### isBehind

```flexscript
int AGV::Trailer.isBehind
```

1 if the trailer is behind its driver AGV, 0 if it is ahead.

### object

```flexscript
Object AGV::Trailer.object (readonly)
```

Gets the 3D object that this trailer is associated with.

### rankFromDriver

```flexscript
int AGV::Trailer.rankFromDriver (readonly)
```

Gets the 1-based rank of this trailer away from its driver AGV.

