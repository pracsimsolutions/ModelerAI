---
id: reference_propertiespanels_conveyorpanels_conveyorbehavior_conveyorbehavior
name: "Conveyor Behavior"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ConveyorPanels","ConveyorBehavior","ConveyorBehavior"]
tags: ["manual","reference","propertiespanels","conveyorpanels"]
source: "manual/Reference/PropertiesPanels/ConveyorPanels/ConveyorBehavior/ConveyorBehavior.html"
---

# Conveyor Behavior

The Conveyor Behavior panel defines the behavior properties for a conveyor.

 

 

The following properties are on this panel:

 

### Accumulating

 

If this box is checked, items will accumulate on the conveyor, meaning they will collect together along the conveyor, similar to a roller conveyor. If this check box is cleared, items will move in lock-step with each other, like on a belt conveyor. By default, this box is checked.

 

### Speed

 

The default speed of the conveyor.

 

### Acceleration

 

The conveyor's rate of speed change as it moves when it first starts or when it changes from a slower speed to a faster speed.

 

### Deceleration

 

The conveyor's speed as it slows down to a stop or when it changes from a faster speed to a slower speed.

  

#### Acceleration and Deceleration

 

Acceleration and deceleration are only applied when the conveyor's speed changes. They are not used when simulating per-item motion such as when an item is blocked by a downstream conveyor or by accumulation.

  

### Stopping Space

 

The minimum amount of space between items while they travel along the conveyor of while they are accumulating. If the space between items is less than the specified space, the item will stop moving.

 

### Moving Space

 

If items are stopped along a conveyor, this is the amount of space needed between items before they can resume movement.

 

### Restart Delay

 

If you define a non-zero restart delay time, after items are stopped on a conveyor, this additional time will be applied before starting the items again after their move space has been cleared.

 

### Entry Space

 

The amount of space that must be clear before an item will enter a conveyor.

  

#### Spacing Rule: Stopping Space ≤ Moving Space

 

A conveyor's moving space must be greater than or equal to its stopping space. If the moving space is equal to the stopping space, items will immediately follow behind items they accumulate against. If the moving space is greater than stopping space (or if the restart delay is greater than zero), items will stop in place when accumulating, and then resume when the moving space is cleared (and the restart delay timer has expired). This results in a caterpillar-like accumulation effect.

   

#### Spacing Rule: Stopping Space ≤ Entry Space

 

A conveyor's entry space must be greater than or equal to its stopping space.

  

Each spacing setting has two different fields, as illustrated in the image below:

 

 

The first editable number field, the **Length Factor** field, allows you to a percentage of space multiplied by the item's length. 1.00 is equivalent to 100%, or in other words the full length of the item. The value 0.5 would be equivalent to 50%, half the item's length. And 2.00 would be equivalent to 200%, twice the item's length.

 

The second editable number field, the **Additional Spacing** field, allows you to add an additional amount of space based on the units of measurements you specify.

 

Using these two fields, you can create a dynamic set of spacing rules that is either relative or absolute to the item's length. The following images show examples and explanations of different spacing settings:

 

| The default of **1.00 x Item Length + 0.00m** defines spacing based purely on item length. |
| --- |
| Defining a spacing value of **1.00 x Item Length + 0.06m** adds a gap of 6 centimeters from the trailing edge of one item to the leading edge of the item behind it. |
| A spacing of **0.00 x Item Length + 0.5m** defines a 0.5 meter spacing from the leading edge of one item, to the leading edge of the next item, independent of item length. |

 

### Power and Free Settings

 

The next group of settings is designed for use in simulating Power and Free conveyor systems. In these systems, dogs travel at fixed intervals along a looping chain. These dogs pick up carriers in the system as they pass them. In simulation terms, it is similar to having a moving space greater than stopping space (there is a caterpillar-like accumulation effect), except that the point at which an item can move on the conveyor is defined by the location of the next passing dog, instead of by the space between it and the item ahead of it.

 

The available settings are:

 

- **Fixed Interval Movement (Power and Free)** - If checked, items can only move on the conveyor at fixed intervals, at the points where simulated dogs pass the item's position.
- **Dog Interval** - The spacing between each dog on the Power and Free chain. If you want to simulate a repeating irregular pattern of gaps between dogs, you can enter in a series of custom numbers separated by commas.
- **Item Edge** - Defines the associated item edge to be picked up by a dog.
- **Catch Condition** - Check this box to enable a custom condition defining whether a given dog will "catch" a given item. One example reason for using this feature might be to simulate randomly missed dogs. Another example would be in order to simulate a cross belt sorter with defined "cells". Larger items take up a full cell, while smaller items can fit two on a cell. Here you would define two dogs per cell, and larger items can only catch the even numbered dogs.

  

#### Initial Dog Positions

 

When Power and Free settings are enabled, by default the conveyor will assign the dog positions based on the first item that enters the conveyor. If you need more control over defining dog positions across multiple conveyors, you can use a motor object.

   

#### Carriers

 

Power and Free conveyors do not simulate carriers automatically. If you need to simulate carriers as limited resources in the simulation, you can simulate them explicitly by having items that are picked up by the dogs represent the carriers, and then use decision points to move other items into and out of those carrier items, representing the actual loads that are being moved by the carriers.

  

### Slug Building Settings

 

The last group of settings allow conveyors to build slugs. Conveyors can build and release slugs as part of a saw-tooth merge.The Slug Building settings are as follows:

 

- **Slug Builder** - Enables slug building when checked.
- **Ready Criteria** - Defines when a slug lane can be considered ready for release. One or more of three options should be chosen. If any of the checked items is fulfilled, the slug will be marked as ready for release. The slug may continue to build while waiting for release. If the conveyor is not connected to a Merge Controller, it will release slugs as soon as they are ready for release.
- **Fill Percent** - Select this box and enter a value to define the percentage of the full conveyor length that is required for the slug to be ready for release.
- **Item Count** - Select this box and enter a value to define the number of items in a slug required to be ready for release.
- **Max Count** - This only applies if **Item Count** is also selected. Select this box and enter a value to define the maximum number of items in a slug. If this is defined, the conveyor will release no more than this number of items in a single slug.
- **Time Elapsed** - Select this box and enter a value to define a maximum elapsed slug-build time. The time starts when the first item is added to the slug. If the slug is still not ready when the timer finishes, the slug will be made ready for release.

  **Release Speed** - The speed at which a slug will be released.
