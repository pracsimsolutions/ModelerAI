---
id: reference_3dobjects_astar_divider_divider
name: "A* Divider"
kind: manual
breadcrumb: ["Reference","3DObjects","AStar","Divider","Divider"]
tags: ["manual","reference","3dobjects","astar"]
source: "manual/Reference/3DObjects/AStar/Divider/Divider.html"
---

# A* Divider

## Overview and Key Concepts

 

An A* divider is an object used to constrain travel on an A* grid. It is defined spatially as a set of connected lines. TaskExecuters are prevented from crossing these lines when traveling. See Working With A* Navigation for more information on how to use A* dividers.

 

### Preferred Paths, Bridges, and Mandatory Paths

 

Preferred paths, bridges, and mandatory paths are subclasses of the divider. They share the commonality that they are all spatially defined as a set of connected lines, and can include a conditional rule, while each subclass defines its own behavior for how it affects TaskExecuter routing. See Working with A* Navigation for more information on adding these specific objects.

 

 

 

## States

 

An A* divider does not implement any automatic state logic.

 

 

 

## Properties

 

An A* divider uses the Divider/Path panel for defining its properties. Below are the set of property names supported by A* dividers.

 

| Property | Type |  |  |
| --- | --- | --- | --- |
| ConditionalRule | Options |  |  |
| TwoWay (all except Bridges) | Boolean |  |  |
| PathPoints | Custom 2-Dimensional Array | PathWeight (Preferred Paths only) | Number |
| UseVirtualDistance (Bridges only) | Boolean |  |  |
| VirtualDistance (Bridges only) | Unit |  |  |
