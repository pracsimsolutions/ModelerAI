---
id: reference_3dobjects_taskexecuters_operator_operator
name: "Operator"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","Operator","Operator"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/Operator/Operator.html"
---

# Operator

## Overview and Key Concepts

 

Operators can be called by objects to be utilized during setup, processing or repair time. They will stay with the object that called them until they are released. Once released, they can go work with a different object if they are called. They can also be used to carry flow items between objects. Operators can be placed on a network if they need to follow certain paths as they travel.

 

 

The operator is a task executer. It implements offset travel depending on whether there's in an involved item for the offset. If there is no item, then it implements the offset exactly like the TaskExecuter object. It travels so that its x/y center and z base arrives at the destination location. If there is an involved item, then the operator only travels along the x/y plane. It also only travels up to the point where its front edge is at the edge of the flow item, instead of its x/y center. This done by decreasing the total travel distance by (xsize(operator) / 2 + xsize(item) / 2).

 

The operator has animations in order to Walk, Walk Loaded, and Stand and more depending on the operator's shape. These animations can be viewed in the Animation Creator by right clicking an operator to open a menu. Point to Edit, then Animations.

 

### Context

 

As mentioned above, the xsize of the item is queried to decrease the total offset distance. This may not work perfectly if you have flow items whose x size is very different from their y size, and the operator approaches from the y side of the flow item. If this is the case, and you want it to look more exact, you could try switching x and y sizes of the item and rotate it 90 degrees right before the operator picks it up, then undo those changes from the operator's load/unload trigger.

 

 

 

## Events

 

The operator uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

This object uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The operator uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Operator object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Person Visuals
- Operator
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Operator object shares several properties with all task executers:

 

- Task Executer Properties

 

Additionally, the Operator object uses the following properties:

 

| Property | Type |
| --- | --- |
| IdleAnimation |  |
| UtilizeAnimation |  |
| WalkAnimation |  |
| WalkLoadedAnimation |  |
