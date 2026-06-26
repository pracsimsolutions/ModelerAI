---
id: astar__cell
name: "AStar::Cell"
kind: class
module: "AStar"
signature: "AStar::Cell"
aliases: []
tags: ["class", "astar-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods associated with an A* grid cell."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AStar\\AStar.Cell.xml"
---

# AStar::Cell

Provides access to data and methods associated with an A* grid cell.

## Methods

### adjacentCell

```flexscript
AStar.Cell AStar::Cell.adjacentCell(int direction)
```

Gets the cell's adjacent cell, in the given direction.

**Parameters:**
- `direction` *(int)* — The desired travel direction. This should be an enumeration of AStar.Direction

### AStar.Cell

```flexscript
AStar.Cell AStar::Cell.AStar.Cell(int grid, int row, int column)
```

Constructs an AStar.Cell based on the grid, row, and column address.

**Parameters:**
- `grid` *(int)* — Reserved for future use. Should be 1.
- `row` *(int)* — The row address.
- `column` *(int)* — The column address.

### canGo

```flexscript
int AStar::Cell.canGo(int direction)
```

Returns true if the travel grid allows a traveler to travel from this cell to an adjacent cell in the defined direction.

**Parameters:**
- `direction` *(int)* — The desired travel direction. This should be an enumeration of AStar.Direction

### getAllocation

```flexscript
AStar.Allocation AStar::Cell.getAllocation(double time = -1)
```

Gets the AStar.Allocation, if any, that is currently active for the cell or is scheduled to be active at a given time.

**Parameters:**
- `time` *(double, default -1)* — The target simulation time.

## Properties

### col

```flexscript
int AStar::Cell.col
```

Gets the cell's column address, or its numbering in the x direction.

Increasing column numbers are associated with increasing x locations.

### row

```flexscript
int AStar::Cell.row
```

Gets the cell's row address, or its numbering in the y direction.

Increasing row numbers are associated with increasing y locations.

