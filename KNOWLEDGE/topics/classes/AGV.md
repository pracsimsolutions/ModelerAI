---
id: agv
name: "AGV"
kind: class
module: ""
signature: "AGV"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related to AGV travel."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.xml"
---

# AGV

Provides access to data and methods related to AGV travel.

The AGV class is associated with a task executer that has been
connected to the AGV network. The AGV is NOT the same as the task executer object. In
other words, you can't just set an AGV to the task executer itself.
TaskExecuter te = current;
AGV agv = te; // RUN-TIME ERROR
Instead, the AGV is associated with, not the same as the task executer. To
access the AGV of a given task executer, use the
AGV() constructor.
That said, an AGV class is actually stored on a treenode, so you can
cast to/from a treenode, pass it around as a Variant, etc. by using the
as() casting method.
return AGV(current).as(treenode);
Also, if you are troubleshooting and want to see this structure as the model is running, you
can view it in the tree at MODEL:/AGVNetwork>variables/agvs/<AGVNAME>,
where <AGVNAME> is the name of the AGV you want to see.

## Methods

### addAllocationPoint

```flexscript
AGV.AllocationPoint AGV.addAllocationPoint(AGV.AllocatableObject object, double stopDist, double deallocDist, double arrivalDist)
```

Adds a new allocation point to the AGVs allocationPoints list.

**Parameters:**
- `object` *(AGV.AllocatableObject)* — The object to be allocated.
- `stopDist` *(double)* — The allocation point's traversalDist, i.e. the point at which to deallocate the object.
- `deallocDist` *(double)*
- `arrivalDist` *(double)* — The allocation point's arrivalDist.

Generally the AGV default behavior properly adds allocation points as part
of the AGV's travel path, so you will usually not need to use this method. However, manually
adding allocations may be useful in in some special
cases. For example, certain spatial configurations of AGVs may require you to allocate
points that are not technically on its travel path. If a part of an AGV 'hangs out' away
from its body and encroaches on other paths, you may resolve this by adding allocation points
on those other paths so that the AGV will avoid collisions with other AGVs.
You should call this method only at defined times in the
AGV's travel operation. Specifically, you should only call it after the OnBuildTravelPath event is
fired (or as part of that trigger) but before the OnPreAllocate. Calling it at any other
time is not guaranteed to work.

### AGV

```flexscript
AGV AGV.AGV(treenode te, int assert = 0)
```

Constructs an AGV object based on the given TaskExecuter.

**Parameters:**
- `te` *(treenode)* — A TaskExecuter that is connected to the AGV network as an AGV Traveler.
- `assert` *(int, default 0)* — If 1, then if the TaskExecuter is not yet connected to the AGV network, it will be connected, and the associated AGV object will be created.

**Returns:** An AGV object

An AGV object can only be accessed by calling this method. Casting directly
from a task executer is not correct.
AGV agv = AGV(te); // correct
AGV agv = te; // incorrect

### attachTrailer

```flexscript
AGV.attachTrailer(treenode trailer, double gap, int behind, int detachRule)
```

Attaches the given object as a trailer on the AGV.

**Parameters:**
- `trailer` *(treenode)* — The object that will trail the AGV
- `gap` *(double)* — The trailer's distance from the next trailer or AGV
- `behind` *(int)* — Sets the position of the trailer. Can be 0 or 1: 0 is ahead, 1 is behind
- `detachRule` *(int)* — Sets the detach mode of the AGV. Can be 0 or 1: 0 is manual, 1 is automatic

If you attach the trailer using the manual detach mode, you must call detachTrailer()
to detach the trailer.
agv.attachTrailer(trailerObj, 0.1, 1, 0);

### coupleTrain

```flexscript
AGV.coupleTrain(treenode trailer)
```

Attaches a train of trailers to this AGV, automatically calculating gaps, etc. based on network geometry.

**Parameters:**
- `trailer` *(treenode)* — The object that will be coupled to the AGV. If the trailer is itself an AGV, or is a trailer of an AGV, then the entire AGV train will be attached.

coupleTrain() is similar to attachTrailer(), with the
following primary differences:

Whereas attachTrailer() attaches a single trailer to the AGV, coupleTrain() will attach all trailers that
are part of another AGV train, if such a train exists.
While attachTrailer() requires you to define parameters like whether the trailer should be attached ahead or
behind, gap distance, etc., coupleTrain(), on the other hand, automatically resolves these
parameters based on the location of the trailer/train on the AGV path network.

There are two possible scenarios when coupling the trailer onto the AGV's train. Each scenario
effects a different logic for coupling the train, as follows:

The trailer is an independent object - When the trailer is not associated with
another AGV train, the AGV will determine where to couple the trailer by analyzing simple spatial
proximity. It retrieves the location of the trailer, and determines whether that location is ahead of
the AGV or behind. Then, based on that determination, it resolves the gap between the trailer and
the AGV's associated ahead/behind-most trailer by again analyzing spatial proximity. Further, since
there is no AGV train associated with the trailer, it will couple only that trailer to the AGV, and
no other objects.
The trailer is itself another AGV, or is a trailer of another AGV - When the
trailer is part of another AGV, the AGV will determine couple parameters by analyzing its own location
on the AGV path network relative to the location of that other AGV on the path network. Ahead/behind
attributes, gap, etc. are determined by doing a path network distance analysis between each end of each
AGV, and coupling the train via the shortest resulting path between the train ends. This algorithm assumes
that the AGV ends can reach each other via one or more paths that do not require a train direction
switch. In other words, the trains' proximity to each on the network should be "close and intuitive",
like how you would want to couple trains in a real life situation. Coupling across weird path
geometries will result in undetermined behavior.

Use this method in conjunction with uncoupleTrain() to simulate coupling and
uncoupling of cars in a train yard.

### detachTrailer

```flexscript
AGV.detachTrailer(treenode trailer)
```

Detaches the given trailer from the AGV.

**Parameters:**
- `trailer` *(treenode)* — The trailer to detach from the AGV

agv.detachTrailer(trailerObj);

### getTrailer

```flexscript
AGV.Trailer AGV.getTrailer(int isBehind, int rankFromDriver)
```

Gets a specific trailer based on an ahead/behind parameter, and a rank for the AGV driver.

**Parameters:**
- `isBehind` *(int)* — 1 if you want to access a trailer behind the AGV, and 0 if you want to access a trailer ahead of the AGV.
- `rankFromDriver` *(int)* — The 1-based rank of the trailer from the AGV driver.

Object trailer = agv.getTrailer(1, 2).object;

### redirect

```flexscript
AGV.redirect(Object newDest, int mode)
```

Redirects the AGV to a new destination, based on the given mode.

**Parameters:**
- `newDest` *(Object)* — The new desination for the AGV to travel to
- `mode` *(int)* — Determines what happens on redirect. Can have one of 4 values: REDIRECT_AND_WAIT - if not redirected before arrival, the agv will wait at the destination until redirected REDIRECT_AS_FINAL - the destination becomes the new final destination for the agv REDIRECT_AND_CONTINUE_ON_ARRIVAL - the agv will continue to the final destination after arriving at the intermediate destination REDIRECT_AND_CONTINUE_ON_PRE_ARRIVAL - the agv will continue to the final destination after pre-arriving at the intermediate destination (it will not decelerate to a stop)

agv.redirect(newDest, REDIRECT_AS_FINAL);

### setDestination

```flexscript
void AGV.setDestination(Object controlPoint, double pathOffset = 0, Object relativeToTrailer = 0, Variant agvBodyOffset = 0)
```

Sets the destination control point.

**Parameters:**
- `controlPoint` *(Object)* — Control point to be set as the destination.
- `pathOffset` *(double, default 0)* — How much to offset the destination along the path from the control point.
- `relativeToTrailer` *(Object, default 0)* — Make the destination relative to a trailer.
- `agvBodyOffset` *(Variant, default 0)* — Specifies amount to offset from the body or one of the AGV.BodyOffset enum values. It may also be an array with 2 elements, the first with one of the AGV.BodyOffset enum values and the second with an additional offset amount.

agv.setDestination(Model.find("ControlPoint5"), 0, 0, AGV.BodyOffset.TrailerHeadEdge);

### startRecharge

```flexscript
double AGV.startRecharge()
```

Begins recharging the AGV's battery.

**Returns:** The amount of time a full recharge will take

The battery recharges at the rate defined by the AGV type. Once the battery is fully recharged, recharging stops.

### stopRecharge

```flexscript
void AGV.stopRecharge()
```

Stops recharging the AGV's battery.

### uncoupleTrain

```flexscript
AGV.uncoupleTrain(treenode trailer, treenode newDriver = 0)
```

Detaches a train of trailers from this AGV, creating a separate AGV train.

**Parameters:**
- `trailer` *(treenode)* — The trailer at which to uncouple from the train.
- `newDriver` *(treenode, default 0)* — The object who will become the new "driver" AGV. If this object does not already have an AGV instance associated with it, then an AGV will be created. If newDriver is not passed as a parameter, or is 0, then the first trailer parameter will become the new driver. If the new driver is not a TaskExecuter, then it will only contain AGV data for storing where the AGV train is on the path network. You will not be able to give the object travel tasks like you would a TaskExecuter.

AGV(current).uncoupleTrain(AGV(current).trailers[1].object);
Use this method in conjunction with coupleTrain() to simulate
coupling and uncoupling of cars in a train yard.

## Properties

### accumAheadAGV

```flexscript
AGV AGV.accumAheadAGV (readonly)
```

Gets the next AGV on an accumulating path.

If there is no AGV ahead of the current one, or if the path type is not
accumulating, this property returns a null value.
AGV agv2;
if (agv1.accumAheadAGV)
agv2 = agv1.accumAheadAGV;

### allocationPoints

```flexscript
AGV.allocationPoints (readonly)
```

An array of AGV.AllocationPoints that is the AGV's current set of not-yet-allocated allocation points.

If you are troubleshooting and want to see this structure as the model is
running, you can view it in the tree at MODEL:/AGVNetwork>variables/agvs/<AGVNAME>/allocationPoints,
where <AGVNAME> is the name of the AGV you want to see.

### ampHours

```flexscript
double AGV.ampHours (readonly)
```

Gets the number of amp hours remaining in the AGV's battery.

double ampHoursLeft = agv.ampHours;

### batteryLevel

```flexscript
double AGV.batteryLevel
```

Gets or sets the ratio of (amp hours remaining)/(total amp hours) as a percentage.

double batteryLevel = agv.batterLevel;

### currentCP

```flexscript
Object AGV.currentCP
```

Gets or sets the current control point for this AGV.

If the agv is not currently on a control point, this will be null.
Object cp = agv.currentCP;
agv.currentCP = Model.find("MyCP");

### currentTravelDist

```flexscript
double AGV.currentTravelDist (readonly)
```

Gets the AGV's current travel distance.

The current travel distance of the AGV within its current travel operation.
The AGV tracks its cumulative travel distance across each of the
sections in its travel path. Each travel operation starts at a travel distance of 0,
and travel distance increases as it travels along the path.

### currentTravelPathSection

```flexscript
AGV.TravelPathSection AGV.currentTravelPathSection (readonly)
```

Gets the travel path section that the AGV is currently traveling on.

### destination

```flexscript
Object AGV.destination (readonly)
```

Gets the current destination of the AGV.

If the destination is a FixedResource connected to a ControlPoint, this property will return the FixedResource.
Object destination = agv.destination;

### destinationCP

```flexscript
Object AGV.destinationCP (readonly)
```

Gets the current desination control point of the AGV.

Object destinationCP = agv.destinationCP;

### finalDestinationCP

```flexscript
Object AGV.finalDestinationCP (readonly)
```

Gets the final destination control point of the AGV.

The final destination control point can be changed when redirect() is called with REDIRECT_AS_FINAL.
Object finalDest = agv.finalDestinationCP;

### isOrientedPathForward

```flexscript
int AGV.isOrientedPathForward
```

Gets or sets whether or not an AGV is oriented "forward" on a path.

You can get this property while an AGV is on a path to see if they are
facing a forward direction. It will return 1 if the AGV is oriented forward and 0 if
they are oriented backwards. You cannot set this property while an AGV is moving.

AGV agv = AGV(Model.find("TaskExecuter1"));
return agv.isOrientedPathForward;

### loadType

```flexscript
Variant AGV.loadType
```

Gets or sets the current load type of the AGV.

Generally you will set the load type as part of the initialize travel trigger.
However, you can also dynamically change an AGV's load type during a travel operation.
This will cause the AGV's speed/acceleration/deceleration profile to change dynamically
while it is performing a travel task. You would do this, for example, if you need to change
the AGV's max speed based upon factors different than just the path that the AGV is
traveling on.

Usually you should not change loadType as part of the execution of some critical trigger in the
AGV's travel operation, such as OnPreArrival. However, you can get around this by using
a Breathe activity in process flow or calling await Delay.seconds(0) in code.

The loadType property is a Variant. You can either set it to the name of the load type,
or you can set it to a number representing the rank (column number) of the load type. When
you get the loadType, it will return the name of the load type.

AGV agv = AGV(Model.find("TaskExecuter1"));
agv.loadType = "Loaded";

### object

```flexscript
Object AGV.object (readonly)
```

Provides access to Object methods and properties for this AGV.

Object object = agv.object;

### originCP

```flexscript
Object AGV.originCP (readonly)
```

Gets the origin control point of the AGV.

Object originCP = agv.originCP;

### overflowCP

```flexscript
treenode AGV.overflowCP
```

Gets/sets the control point to "overflow" to when the AGV is pushing trailers.

This is generally only needed if the AGV is pushing trailers. When pushing trailers,
overflowCP defines a point on the AGV path network to which the AGV's trailers should "overflow",
since those trailers will be pushed past the AGV's target destination.
overflowCP is controlled solely by the user. It is only automatically reset when the model is reset.
Thus, if you set this property, you should reset it to 0 after finishing the operation where the
it is applicable. If you do not reset it after its applicable travel operation, weird AGV path travel
behavior may occur.
agv.overflowCP = Model.find("ControlPoint5");

### proximityState

```flexscript
int AGV.proximityState (readonly)
```

Gets the current proximity state of the AGV.

A 0 indicates unrestricted. A 1 indicates that the AGV is stopped because of the AGV ahead of it.
if (agv.proximityState) {
// handle a stopped agv
...
}

### recentCP

```flexscript
Object AGV.recentCP
```

Gets the control point that was most recently the agv's currentCP.

This is useful if the agv has moved past its last currentCP (and therefore
currentCP is null) but you still want to be able to access that most recent control point.
Object cp = agv.recentCP;

### resetCP

```flexscript
Object AGV.resetCP
```

Gets or sets the control point that the agv connects to when the model is reset.

Object cp = agv.resetCP;
agv.resetCP = Model.find("MyCP");

### speed

```flexscript
double AGV.speed (readonly)
```

Gets the current speed of the AGV.

double agvSpeed = agv.speed;

### te

```flexscript
TaskExecuter AGV.te (readonly)
```

Provides access to TaskExecuter methods and properties for this AGV.

TaskExecuter te = agv.te;

### trailers

```flexscript
AGV.trailers (readonly)
```

An array of AGV.Trailers comprising the set of trailers attached to this AGV.

The trailers are ordered from front to back. In other words the ahead-most
trailer will be agv.trailers[1], and the behind-most trailer will be
agv.trailers[agv.trailers.length].

### travelPath

```flexscript
AGV.travelPath (readonly)
```

An array of AGV.TravelPathSections that is the AGV's travel path for its current travel operation.

If you are troubleshooting and want to see this structure as the model is
running, you can view it in the tree at MODEL:/AGVNetwork>variables/agvs/<AGVNAME>/travelPath,
where <AGVNAME> is the name of the AGV you want to see.

### traversalPoints

```flexscript
AGV.traversalPoints (readonly)
```

An array of AGV.TraversalPoints that is the AGV's current set of planned 'traversals'.

For the purposes of the FlexScript API, the traversalPoints are the set of allocation
points that have been allocated but not yet deallocated, i.e. the AGV's current allocations.
See AGV.TraversalPoint for more information.
If you are troubleshooting and want to see this structure as the model is
running, you can view it in the tree at MODEL:/AGVNetwork>variables/agvs/<AGVNAME>/traversalPoints,
where <AGVNAME> is the name of the AGV you want to see.

