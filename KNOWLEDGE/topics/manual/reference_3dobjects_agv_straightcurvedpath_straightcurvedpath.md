---
id: reference_3dobjects_agv_straightcurvedpath_straightcurvedpath
name: "Straight and Curved Path"
kind: manual
breadcrumb: ["Reference","3DObjects","AGV","StraightCurvedPath","StraightCurvedPath"]
tags: ["manual","reference","3dobjects","agv"]
source: "manual/Reference/3DObjects/AGV/StraightCurvedPath/StraightCurvedPath.html"
---

# Straight and Curved Path

## Overview and Key Concepts

 

AGV Paths define the routes that AGVs take to get to their destinations on the AGV network.

 

### Straight vs Curved

 

A Path can either be a straight path:

 

 

Or a curved path:

 

 

### One-Way vs Two-Way

 

A Path can either be one-way:

 

 

Or two-way:

 

 

This shape shows you that the path is two-way, but also shows you the *primary* direction of the path. Most of the time the primary direction of the path does not matter. It is only needed if you give an AGV a travel task with an offset along the destination path, in which case the offset is a distance along the path's primary direction.

 

### AGV Orientation

 

You can also force a single AGV orientation for a path. This defines how the AGV must face while on the path, irrespective of the AGV's travel direction. The default is any, meaning the AGV may face any direction while on this path. You can define it as forward-only:

 

 

Or backward-only:

 

 

Think of the half-square as an outline of the AGV's front end. Paths with a defined AGV orientation can be two-way, but the Forward/Backward-ness of the AGV Orientation is relative to the direction the Path would be if it were one-way.

 

Usually you can enforce proper AGV orientation simply by building the path network correctly. However, in some cases this is not possible, hence you have the option to enforce it explicitly. Note that choosing an AGV orientation on any path in your network will cause a non-trivial amount of extra memory to be allocated to build routing tables for each AGV orientation, and if you haven't built your network properly it may cause routing failures because the network can't find a path that will get the AGV to the destination with the proper orientation. Hence it is advisable to only use this option if it is explicitly needed.

 

### Path Classes

 

Path Classes allow you to break out AGV speeds by path.

 

### Accumulation Types

 

Each path can have an Accumulation Type assigned to it. If defined (not No Accumulation), AGVs traveling on that path will detect proximity with each other and avoid collisions. You can define Accumulation Types in the AGV Network's Accumulation Types tab.

 

 

 

## Properties Panels

 

The Curved Path object uses the following properties panel:

 

- Curved Path

 

The Straight Path object uses the following properties panel:

 

- Straight Path

 

 

 

## Properties

 

The path objects use the following properties:

 

| Property | Type |
| --- | --- |
| AGVOrientation | Options |
| AccumulationType | Options |
| IsTwoWay | Boolean |
| PathClass | Options |
| EndLocation | Array(3) |
| StartLocation | Array(3) |

 

Additionally, the curved path objects use the following properties:

 

| Property | Type |
| --- | --- |
| Radius | Unit |
| StartAngle | Unit |
| SweepAngle | Unit |
