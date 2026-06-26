---
id: astar__direction
name: "AStar::Direction"
kind: class
module: "AStar"
signature: "AStar::Direction"
aliases: []
tags: ["class", "astar-module"]
deprecated: false
replacedBy: null
description: "An enumeration of integer values associated with four A* travel directions."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AStar\\AStar.Direction.xml"
---

# AStar::Direction

An enumeration of integer values associated with four A* travel directions.

The AStar.Direction values are used in AStar.Cell when querying
directional travel capabilities and adjacent cells.

## Properties

### Down

```flexscript
int AStar::Direction.Down (readonly)
```

The "down" direction, describing travel in the negative y direction.

### Left

```flexscript
int AStar::Direction.Left (readonly)
```

The "left" direction, describing travel in the negative x direction.

### Right

```flexscript
int AStar::Direction.Right (readonly)
```

The "right" direction, describing travel in the positive x direction.

### Up

```flexscript
int AStar::Direction.Up (readonly)
```

The "up" direction, describing travel in the positive y direction.

