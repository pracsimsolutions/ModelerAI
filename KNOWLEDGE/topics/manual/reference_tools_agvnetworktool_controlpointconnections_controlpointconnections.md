---
id: reference_tools_agvnetworktool_controlpointconnections_controlpointconnections
name: "Control Point Connections"
kind: manual
breadcrumb: ["Reference","Tools","AGVNetworkTool","ControlPointConnections","ControlPointConnections"]
tags: ["manual","reference","tools","agvnetworktool"]
source: "manual/Reference/Tools/AGVNetworkTool/ControlPointConnections/ControlPointConnections.html"
---

# Control Point Connections

## Overview and Key Concepts

 

Control point connections are logical named connections between control points. They associate control points with each other *logically* (as opposed to paths, which associate control points *navigationally*).

 

While control point connections have "best-practice" names and meanings, the specific meaning of control point connections (i.e. how specific control connections drive the logical execution of a model), are not hard-coded into the internal AGV navigation logic. Their meanings are ultimately dependent on how you, or the process flow you decide to use, imbue these control point connections with meaning. The result is an incredibly flexible and powerful way to define AGV logic in FlexSim.

 

See Building AGV Network Logic > Control Point Connections for more information.   

## The Control Point Connections Tab

 The Control Point Connections tab has the following properties:

 

 

You can get to the Control Point Connections page by right-clicking on a Path or Control Point and choosing **AGV Network Properties**.

 

For each Control Point Connection, you can define whether or not its visible in the 3D view, and its color. You can also add, remove, and rename connections as needed.

 

### Using Lists

 

If you check the List box for a connection, all of those connections in the model will be added to a global partitioned list. This allows you to easily define AGV control logic with a process flow. When an AGV arrives at a given destination, you can decide what to do by pulling from various list partitions associated with the AGV's current control point, such as, find a control point near here that has work that the AGV can do, or find a parking spot to go to, or continue to the next point to look for work. These operations can be done by pulling from the current control point's partition of the global list.

 

### Connection Types

 

A Control Point Connection has one of three possible connection types:

 

- **Control Point One-Way** - This connection type means the connection is a one-way connection from one Control Point to another Control Point. In other words, when you make one of these connections from Control Point A to Control Point B, Control Point B will not have a connection back to Control Point A.
- **Control Point Two-Way** - This connection type means the connection is a two-way connection from one Control Point to another Control Point. In other words, when you add one of these connections from Control Point A to Control Point B, a connection from Control Point B back to Control Point A will automatically be created as well. When you add one of these connection types, you can make it so that the reverse connection (B back to A) connects back using the same connection type (the To Self option), or using a new connection type. Technically the two-way connection doesn't give you any added capability over the one-way because you can do this same functionality using one-way connections by manually adding one-way connections in both directions. However, this can save time in creating the connections if needed.
- **Object Two-Way** - This connection type represents a two-way connection from a Control Point to a model object. When you add one of these connections, the Control Point can accesss the object through that connection, and the object can access the control point through the same connection.
