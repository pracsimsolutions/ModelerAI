---
id: astar__allocation
name: "AStar::Allocation"
kind: class
module: "AStar"
signature: "AStar::Allocation"
aliases: []
tags: ["class", "astar-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods associated with an A* grid cell allocation."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AStar\\AStar.Allocation.xml"
---

# AStar::Allocation

Provides access to data and methods associated with an A* grid cell allocation.

## Properties

### acquireTime

```flexscript
double AStar::Allocation.acquireTime (readonly)
```

Gets the time that the cell was/will be allocated.

### cell

```flexscript
AStar.Cell AStar::Allocation.cell (readonly)
```

Gets the allocated cell.

### releaseTime

```flexscript
double AStar::Allocation.releaseTime (readonly)
```

Gets the time that the cell was/will be released.

### traveler

```flexscript
AStar.Traveler AStar::Allocation.traveler (readonly)
```

Gets the allocating traveler.

