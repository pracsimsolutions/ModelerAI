---
id: reference_tools_astar_astar
name: "A* Navigation"
kind: manual
breadcrumb: ["Reference","Tools","AStar","AStar"]
tags: ["manual","reference","tools","astar"]
source: "manual/Reference/Tools/AStar/AStar.html"
---

# A* Navigation

## Overview and Key Concepts

 

A* (pronounced "A Star") is a well-known [search algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm) used to find optimals paths between points. The algorithm uses a grid of nodes that Task Executer objects (travel members) travel through. Each node specifies in which direction travel members can move. The algorithm will look at nodes in the direction of travel and determine which direction is the fastest, including traveling diagonally between nodes. The grid of nodes are modified by creating barriers restricting where the travel members can move or even influencing them to travel along certain paths.

 

 

 

## The General Tab

 

The General tab has the following properties:

 

 

### Surround Depth

 

This number specifies the number of nodes that are placed around the outside of the navigator's outer most barriers. This number must be an integer and be greater than 0.

 

### Smooth Rotations

 

When checked, travelers will smoothly rotate when turning.

 

### Snap Dividers to Grid

 

When checked, dividers will snap between points of the A* grid.

 

### Cache Paths

 

When checked, the A* Navigator will cache paths to be reused in order to save processing time. Note that the A* Navigator can only cache paths if you are not using dynamic barriers. If you need path caching, you should use conditional rules if possible.

 

### Members

 

The box on the left selects which members lists to view. Clicking the Properties button opens the properties window of the selected member. The box on the right displays all of the objects at the currently selected status.

 

| Icon | Description |
| --- | --- |
|  | Click to sample an object in the model to add as a member. |
|  | Click to open an object selection window and choose objects to add as members. |
|  | Remove the selected member. |
|  | Center the view on the selected member. |

 

 

 

## The Behavior Tab

 

 

### Path Routing

 

Defines how nodes in the travel grid will be searched. Options are as follows:

 

- **Right Angles Only** - The A* path search will only find paths that travel vertically or horizontally. No diagonal travel will be used.
- **Diagonals** - The A* path search will allow for diagonal travel. Travelers can simultaneously travel one unit vertically and one unit horizontally.
- **Deep Diagonals** - The A* path search will allow for deep diagonal travel. Travelers can simultaneously travel two units vertically and one unit horizontally, or vice versa.

 

### Strict Diagonals

 

If checked, the traveler will only travel diagonals if the grid travel directions allow the traveler to travel using both direction orderings, namely both "x then y" and "y then x", to get to the diagonal destination. Checking this box will ensure that travelers don't overshorten their path when going around corners.

 

### Collision Avoidance

 

When checked, travelers will allocate nodes in their path and deallocate them once passed. This prevents two travelers from traveling through the same nodes at the same time.

 

### Ignore Inactive Memebers

 

When checked, the collision avoidance algorithm will ignore allocations made by inactive travelers. After a traveler has arrived at its destination and ended active travel, the traveler will be essentially invisible to the collision detection algorithm. This means that travelers will run over other travelers that are not actively traveling.

 

While travelers running over each other is not desirable, it is somewhat complex to figure out what a traveler should do when it is idle to get out of the way of other travelers who need to go to the same spot. In a humans-as-travelers situation, generally people just figure out how to make space for each other without much delay penalty. From this perspective of delay penalties, letting idle travelers ignore each other essentially simulates the same desired behavior as humans naturally getting out of each other's way.

 

### Deallocation Time Offset

 

Defines an additional time to wait before deallocating a grid node that a traveler is leaving. By default (0.00) a traveler will release its node as soon as it begins to travel to the next node. When this value is greater than zero, the traveler will wait this additional time before releasing the node.

 

### Stop for Turns

 

When checked, travelers who need to turn to a new direction will stop and rotate to the new direction.

 

### Turn Speed

 

Defines the turn speed to use when travelers stop and rotate for turns.

 

### Turn Delay

 

Defines an additional time delay associated with stopping for turns. When the traveler must stop and turn, it will add this delay time to the time it takes to the turn to the new direction.

 

### Route By Travel Time

 

If checked, the A* search algorithm will use travel time instead of travel distance as the cost function in assessing path desirability. The primary difference in using this is that, when travelers stop for turns, it will calculate the time it takes to turn, and apply that to the cost function.

 

### Default Preferred Path Weight

 

This is the default path weight for any preferred paths without a custom weight. Valid values are numbers between 0 and 1. Setting a path to have a path weight of 0 will revert the path to using this default value.

 

 

 

## The Visual Tab

 

 

Use these properties to turn on or off the visiblilty of A* features.

 

### Barriers

 

When checked, Barriers will be visible.

 

### Grid Bounds

 

When checked, a blue box is drawn indicating the bounds of the grid.

 

### Grid Nodes

 

When checked, displays the individual nodes of the grid. These are yellow lines that indicate the directions a traveler can move from that node.

 

### Members

 

When checked, displays a blue circle under travelers and a blue rectangle under Fixed Resources connected to the A* Navigator.

 

### Allocations

 

When checked, lines are drawn from travelers to the nodes they have allocated.

 

### Heat Map

 

When checked, a heat map is generated based on the option selected in the dropdown. The Max Heat Value corresponds with the selected dropdown option and defines the threshold for coloring the heatmap.

 

### Travel Threshold

 

When checked, red and blue diamonds will be displayed around the highlighted object in the 3D view. When a traveler is given a travel task to travel to this object, the A* Navigator will compute the shortest path between the traveler and any of the nodes marked by a red diamond. The traveler will then begin traveling following this shortest path. As soon as the traveler reaches any of the nodes marked by a blue diamond the traveler has arrived at the object and the travel task is complete.

 

 

### Debug/Show Routing Algorithm

 

This checkbox is used primarily for debugging purposes. When checked, the A* search algorithm will save additional data that tracks each traveler's last path finding operation. To see this information:

 

1. Check the Debug/Show Routing Algorithm box.
1. Press the Apply button.
1. Run the model.
1. Click or shift-select a traveler. When you select the traveler, you'll see a set of dots on the A* grid that are associated with that traveler's last path finding operation: - Green dots represent nodes that are in the A* *open set.* - Red dots represent nodes in the *closed set.* - The blue dot shows the node that is being resolved from the open set to the closed set. - Text also appears beside each node showing the formula g + h = f, where g is the cost to get to a node, h is the heuristic calculation to get from the node to the destination, and f is the sum of the two.
1. Adjust the Completion Percent tracker to see the routing algorithm as it progresses.

 

 

 

## The Conditional Rules Tab

 

 

Use this tab to define conditional rules by which barriers, dividers, preferred paths, or bridges will be applied in the routing algorithm.

 

 

 

## A* Grids

 

A*'s path search uses grids to resolve travel paths. A grid is a two-dimensional plane that encompasses a travel area. After you've dragged barriers, dividers, etc. into a model, if you reset the model, you will see a grid represented by a blue rectangular border surrounding those objects. The grid is a two-dimensional array of nodes, with a user-defined distance between each node. A* travelers move by traveling from one node in the grid to another. Barriers and dividers determine which adjacent nodes may or may not be traveled to from a given node.

 

Every A* model has at least one grid. That grid will automatically grow to encompass all the barriers, dividers, preferred paths, etc. that are part of your A* model. Also, you can add additional grids by dragging them from the library on the left. You can move and resize grids in the 3D view as you would any other object. You can also define the node spacing of a grid, as well as its z position, through its Properties pane.

 

The Grid object uses the following properties panel:

 

- Grid

 

 

 

## Divider/Path Properties Panels

 

The Divider and Path objects use the following properties panel:

 

- Divider/Path

 

 

 

## Barrier Properties Panels

 

The Barrier object uses the following properties panel:

 

- Barrier

 

 

 

## Node Allocation / Collision Avoidance

 

If you have checked the **Collision Avoidance** checkbox, the A* module will use a node allocation mechanism to ensure objects do not collide with each other. When a traveler begins a task to travel to a new destination, it will perform the following high-level logic:

 

1. **Calculate the Path** - The logic will first use the standard [A* search algorithm](https://en.wikipedia.org/wiki/A*_search_algorithm) to resolve a path from the traveler's current location to the destination, via adjacent nodes in the A* grid.
1. **Build Kinematics / Schedule Node Allocations** - Next the algorithm will build kinematics to travel that path. As the algorithm builds kinematics, it 'schedules' node allocations along the path. Essentially it will store data saying, 'Task Executer Y needs to allocate node X from time A to time B'. So every node will have data on it saying: from time A to time B, I'll be allocated by TE Y, from time C to time D I'll be allocated by TE Z, etc.
1. **Create Next Event** - As long as it doesn't find any allocation time 'collisions' in building out these kinematics (and there are no bridges on the path), then it will just build the kinematics for the full path, schedule the allocation timings, and create the final event for when it arrives at the destination. However, if it finds a time collision as it schedules the allocations, the TE that allocates the node later will create an onBlock event for the time that the node would need to be allocated. When this block event happens, the object will go into a blocked state, wait until the node is released, and then start the whole process over from its current point.

 

The node allocation mechanism only allocates nodes that are on the traveler's path, as well as nodes that are between diagonally adjacent nodes if you have configured travelers to travel diagonally. This means that if you want this allocation algorithm to properly avoid collisions between travelers, your node size must be large enough to fit an entire traveler. If your grid node size is too small, then travelers will overlap. Alternatively, you can disable the A* node allocation mechanism, and implement your own collision avoidance scheme using the A* network in conjunction with the Agent module.

 

 

 

## Acceleration / Deceleration

 

The A* network also supports acceleration and deceleration for objects traveling on the network. Generally this is intended to enable simulation of free-roaming autonomous mobile robots, not necessarily people. If you are simulating people, such as in the people module, some of the effects of using acceleration / deceleration may be disadvantageous for that scenario (see below), so it may be better to just set their acceleration and deceleration properties to 0. The Create Person activity automatically does this when attaching a created person to the A* network.

 

Things to consider when using acceleration / deceleration:

 

- **Node Allocation Timing** - When travelers have non-zero deceleration, it changes the timing of node allocation. Travelers will allocate a node at the point in time when, if the allocation fails, the traveler will still have space to decelerate to stop one grid node short of the failed allocation node. The result is that, for smaller non-zero decelerations, the travelers will allocate much farther ahead, blocking the path for others.
- **Non-Zero End Speeds** - When a travel/offset/load/unload task has a non-zero end speed, the traveler will actually finish the task early, before the traveler has fully arrived at the destination. This is to prevent the objects from 'hitting a wall', i.e. being forced to instantaneously stop when they are not immediately given a subsequent task or are blocked from moving forward. Instead they allocate farther ahead, and then finish the task early so they can decelerate to arrive at the destination. If they are immediately given a new task, they'll start it from the shortened point where they finished the previous task.

 

 

 

## A* Traveler Events

 

When you attach a Task Executer to the A* network, the object will acquire several additional events:

 

### On AStar Block

 

Fired when the traveler is the later allocator of a grid node allocation time collision, at the point when the traveler would otherwise need to allocate the node.

 

### On AStar Reroute

 

Fired when a traveler has blocked, and the blockage has created a deadlock cycle of allocation requests. Here the A* module runs logic to divert one of the members of the deadlock cycle to go around the deadlock.

 

### On AStar Continue

 

Fired when a traveler may continue from a blocked state, i.e. when the node it is attempting to allocate has been released.

 

### On AStar Bridge Arrival

 

Fired when a traveler arrives at an A* bridge.

 

### On AStar Bridge Continue

 

Fired when a traveler continues from an A* bridge.

 

### On AStar Calculate Path

 

Fired when a new path is calculated to a destination.

 

### On AStar Navigate Path

 

Fired when a traveler has built its kinematics and scheduled its node allocations, either to the final destination or to its next 'event' point, e.g. a block point or a bridge arrival point.
