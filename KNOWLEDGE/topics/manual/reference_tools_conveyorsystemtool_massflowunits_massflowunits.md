---
id: reference_tools_conveyorsystemtool_massflowunits_massflowunits
name: "Mass Flow Units"
kind: manual
breadcrumb: ["Reference","Tools","ConveyorSystemTool","MassFlowUnits","MassFlowUnits"]
tags: ["manual","reference","tools","conveyorsystemtool"]
source: "manual/Reference/Tools/ConveyorSystemTool/MassFlowUnits/MassFlowUnits.html"
---

# Mass Flow Units

## Overview and Key Concepts

 

Mass flow units, or just *flow units*, are the elements whose flow is simulated by mass flow conveyors. In FlexSim, mass flow conveyors use a fluid-like algorithm for calculating product flow. Using this algorithm enables significant simulation performance improvements. Whereas the algorithm represents a computational simplification, in real life, mass flow conveyors convey actual discrete objects like bottles, cans, crackers, cookies, cartons, etc. The flow unit in FlexSim provides the data necessary to translate between these discrete objects and the fluid-like algorithm that simulates their flow in FlexSim.

 

 

The primary simulation-defining properties of the flow unit are its **Length**, **Width**, and **Round Shape**. Using these attributes, the simulation can resolve the maximum possible density of units on a conveyor. These densities are the actual things that flow down conveyors, instead of individual units. Using the calculated max density, each conveyor can calculate flow rates based on the conveyor's width and speed.

 

The other attributes of the flow unit are for rendering purposes. They define how the units should be drawn on the conveyor.

 

 

 

## Properties

 

The Mass Flow Unit Properties window has the following properties:

 

### Length

 

The length of a unit. This corresponds to the space taken up by a single unit, along the conveyor's forward axis. If you are simulating bottles, this would be the diameter of the bottle. If you are simulating a non-symmetric object like, say, crackers, this should be the amount of forward-axis space the cracker takes up when accumulated on the conveyor.

 

### Width

 

The width of a unit. This corresponds to the space taken up by a single unit, along the conveyor's left-right axis. If you are simulating bottles, this would be the diameter of the bottle. If you are simulating a non-symmetric object like, say, crackers, this should be the amount of left-right-axis space the cracker takes up.

 

### Height

 

The height of a unit. This is only used for drawing purposes, and defines the height at which units, bars, etc. are drawn on the conveyor.

 

### Round Shape

 

Defines whether the unit's shape is round. When this is checked, the maximum density of the unit will be calculated higher, as round shapes like bottles will pack together using a hexagonal, as opposed to a square, pattern. This also causes units to be drawn on the conveyor using this hexagonal pattern.

 

### Draw Mode

 

Defines how the various densities will be drawn on the conveyor. One of the following values is possible:

 

- **Shapes** - The units will be drawn as individual shapes. Note that it does not actually simulate/track individual units. It merely draws a number of units based on the unit density at a given point on the conveyor. This setting is also further configured with the **Fill Order** property, explained below.
- **Bars** - The densities will be drawn as bars that flow along the conveyor. This setting is also further configured with the **Fill Order** property, described below.

 

### Shape Item

 

When **Draw Mode** is defined as *Shapes*, this defines the shape to be drawn. You can statically define one of the flow items, or you can dynamically define different shapes based on things like densities, etc. This uses just the basic 3D shape of the flowitem and not the full composite flowitem visuals.

 

### Fill Order

 

Defines how different densities will "fill up" the conveyor when drawn. This is dependent on the **Draw Mode**. The following table demonstrates the different property combinations, and how they will be drawn.

 

|  | Shapes | Bars |
| --- | --- | --- |
| Length then Width |  |  |
| Width then Length |  |  |
| Randomized |  |  |

 

The Randomized setting is intended to render units in a more realistic randomly distributed way. This is only a visual approximation. There may be overlapping units in high densities, and individual bottles may disappear and reappear when moving across conveyors in very low densities.
