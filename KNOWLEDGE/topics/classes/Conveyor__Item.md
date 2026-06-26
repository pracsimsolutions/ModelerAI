---
id: conveyor__item
name: "Conveyor::Item"
kind: class
module: "Conveyor"
signature: "Conveyor::Item"
aliases: []
tags: ["class", "conveyor-module"]
deprecated: false
replacedBy: null
description: "A class that provides properties and methods for monitoring and controlling an item on a conveyor."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Conveyor\\Conveyor.Item.xml"
---

# Conveyor::Item

A class that provides properties and methods for monitoring and controlling an item on a conveyor.

The Conveyor.Item is NOT the same as the item object. In other words, you can't just set a Conveyor.Item to the item itself.
Object item = param(2);
Conveyor.Item conveyorItem = item; // COMPILE ERROR
Instead, the Conveyor.Item is associated with, not the same as the item. It contains
data associated with a specific item's conveyance on a specific conveyor. Thus, you
need both the conveyor and the item to get access to the Conveyor.Item.

You get access to Conveyor.Item using the Conveyor.itemData
property.

Object item = param(2);
Conveyor conveyor = param(3);
Conveyor.Item conveyorItem = conveyor.itemData[item];

## Methods

### resume

```flexscript
void Conveyor::Item.resume(int force = 0)
```

Resumes a stopped item's movement on the conveyor.

**Parameters:**
- `force` *(int, default 0)* — If 1, the method will ignore the accumulation of stop() calls, and immediately start the item moving.

Paired with stop(). Calls to stop() will accumulate, meaning
if you call stop() twice on the same item, by default you must call resume() twice to actually resume the item.

### stop

```flexscript
void Conveyor::Item.stop()
```

Stops the item's movement on the conveyor.

Paired with resume(). Calls to stop() will accumulate, meaning
if you call stop() twice on the same item, by default you must call
resume() twice to actually resume the item.

### turn

```flexscript
void Conveyor::Item.turn(int axis, int clockwise)
```

Turns the item 90 degrees along a given axis, and re-configures the item's length, width, height, and spacing values.

**Parameters:**
- `axis` *(int)* — The conveyor axis to turn the item on. This can be on of the following: CONV_AXIS_UP - the conveyor's up axis CONV_AXIS_LEFT - the conveyor's left axis CONV_AXIS_FORWARD - the conveyor's forward axis
- `clockwise` *(int)* — Should be 1 or 0, defining whether to turn the item clockwise around the given axis.

The turning logic assumes there is enough space around the item that it will not overlap another
item once turned. Behavior is undefined if there is not enough space.

## Properties

### currentDistance

```flexscript
double Conveyor::Item.currentDistance (readonly)
```

Gets the item's current distance on the conveyor.

This is the same as position.x, except that
offset kinematics are not considered here.

### destination

```flexscript
Object Conveyor::Item.destination
```

Gets/sets the item's current target destination.

If you set this, it should be set to a point on the conveyor system, such
as a decision point or photo eye. See also Conveyor.sendItem()

### entrySpace

```flexscript
double Conveyor::Item.entrySpace (readonly)
```

Gets the item's defined entry space.

### movingSpace

```flexscript
double Conveyor::Item.movingSpace (readonly)
```

Gets the item's defined moving space.

### object

```flexscript
Object Conveyor::Item.object
```

Gets the object (the flow item) associated with this Conveyor.Item.

### orientation

```flexscript
int Conveyor::Item.orientation
```

Gets/sets the item's orientation on the conveyor.

Setting this value sets the item's orientation, and re-configures the item's length, width, height,
and spacing values. The orientaton parameter defines one of 24 possible orientations for the item. It
is a macro of the format CONV_ITEM_M_BY_N, where M is the axis of the item that points forward along the conveyor, and N
is the axis pointing left along the conveyor. M must not be the same as N. M and N can be one of the following:

X - item positive x axis
X_NEG - item negative x axis
Y - item positive y axis
Y_NEG - item negative y axis
Z - item positive z axis
Z_NEG - item negative z axis

Examples:
CONV_ITEM_X_BY_Y - This is the default item orientation, which is that the item's positive x axis is aligned with
the conveyor's forward axis, and the item's positive y axis is aligned with the conveyor's left axis. If you were
to imagine a person traveling down the conveyor, in this orientation the person would be standing up facing
forward on the conveyor.
CONV_ITEM_Z_NEG_BY_Y - Here the item's negative z axis is aligned with the conveyor's forward axis, and the item's
positive y axis is aligned with the conveyor's left axis. If this were a person, he would be lying on his back
traveling feet first down the conveyor.
CONV_ITEM_Y_NEG_BY_Z_NEG - Here the item's negative y axis is aligned with the conveyor's forward axis, and the item's
negative z axis is aligned with the conveyor's left axis. If this were a person, he would be lying on his back,
sideways with his right side leading down the conveyor.

### position

```flexscript
Vec3 Conveyor::Item.position (readonly)
```

Gets the item's current position on the conveyor.

This is associated with the center of the item. x is distance from
the start of the conveyor, y is distance offset from the center of the conveyor,
and z is vertical distance from the conveyor plane.
If kinematics have been added to the item using the timeKinematics
and/or travelKinematics properties, position will reflect those applied
kinematics.

### rotation

```flexscript
Vec3 Conveyor::Item.rotation (readonly)
```

Gets the item's current offset rotation on the conveyor.

The rotation elements will only be non-zero if you have applied kinematics to the item using the
timeKinematics and/or travelKinematics properties.

### size

```flexscript
Vec3 Conveyor::Item.size (readonly)
```

Gets the item's size on the conveyor.

An item's size on the conveyor is dependent on its orientation, and thus may not
correspond directly with the item object's x, y and z sizes. The x element corresponds to the item's size along the conveyor's forward
axis, y corresponds to the item's size along the conveyor's left axis, and z corresponds to the item's size along the conveyor's up axis.

### stoppingSpace

```flexscript
double Conveyor::Item.stoppingSpace (readonly)
```

Gets the item's defined stopping space.

### timeKinematics

```flexscript
treenode Conveyor::Item.timeKinematics (readonly)
```

Gets a reference to a time-based kinematics node associated with the item.

This is for use in
defining animations and offsets for the item that are dependent on simulation time. Get the node, then
use kinematics commands to add time-based movement to the item. Note that kinematics-based
offsets do not affect standard conveying logic like accumulation, photo eye cover/clear points, etc. The
conveyor system applies kinematics offsets only visually. This means you will likely not want to
offset the item in the x direction (forward or backward along the conveyor), unless you are willing to have
accumulation and other conveyor logic fire at weird points visually.

### totalDistance

```flexscript
double Conveyor::Item.totalDistance (readonly)
```

Gets the item's current total distance traveled within the conveyor system.

Usually this is only used when adding kinematics to the item's travel-based kinematics using the
travelKinematics property, as the "start time" for
adding to the kinematics corresponds to the total distance traveled.

### travelKinematics

```flexscript
treenode Conveyor::Item.travelKinematics (readonly)
```

Gets a reference to a travel-based kinematics node associated with the item.

This is for use in
defining animations and offsets for the item that are dependent on conveyor distance. Get the node,
then use kinematics commands to add travel-based movement to the item. When adding kinematics
to this node, start times, speeds, and accelerations should not be in simulation time, but should rather be
defined in "distance" units. Start times should be relative to the item's
totalDistance. For example, if you add a kinematic that starts at "time" 10, and travels
at "speed" 5, this means that the kinematic will start when the item reaches travel position 10, and
will travel 5 units of distance for every distance unit that the item conveys. Note that kinematics-based
offsets do not affect standard conveying logic like accumulation, photo eye cover/clear points, etc. The
conveyor system applies kinematics offsets only visually. This means you will likely not want to
offset the item in the x direction (forward or backward along the conveyor), unless you are willing to have
accumulation and other conveyor logic fire at weird points visually.

