---
id: agv__traversalpoint
name: "AGV::TraversalPoint"
kind: class
module: "AGV"
signature: "AGV::TraversalPoint"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "A structure containing information about a planned AGV 'traversal'."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.TraversalPoint.xml"
---

# AGV::TraversalPoint

A structure containing information about a planned AGV 'traversal'.

Traversal points are primarily accessed through the
AGV.traversalPoints and
AGV.allocationPoints array. The AGV's
traversalPoints array is a direct list of AGV.TraversalPoint's. The AGV's allocationPoints
array is a list of AGV.AllocationPoint's, which is a subclass of
AGV.TraversalPoint, and as such includes the properties of AGV.TraversalPoint.
This class represents a point along the AGV's travel path where the AGV is scheduled to execute
some logic. The AGV's
traversalPoints array may contain several different types of traversal points, but for the
purposes of the FlexScript API at this time, you should only access/manipulate traversal points
that are instances of AGV.AllocationPoint. In traversing this array you
should always check if the point is in fact an allocation point, and only do something if it is.

AGV agv = AGV(current);
for (int i = 1; i <= agv.traversalPoints.length; i++) {
AGV.TraversalPoint t = agv.traversalPoints[i];
if (t.is(AGV.AllocationPoint)) {
AGV.AllocationPoint allocPoint = t;
// ... perform some logic on the allocation point
}
}
Whereas the AGV.allocationPoints
array contains the set of allocation points that have yet to be allocated, an AGV's
traversalPoints array holds, among other things, the set of allocation points that the AGV
has already allocated, but has not yet deallocated. In other words, it is the AGV's current
set of allocations.
An allocation point will
start out as part of the allocationPoints array. Then, when the AGV actually allocates the object
associated with that allocation point, the allocation point is moved into the traversalPoints
list. At that point, the traversalDist
property of the allocation point is used as the
'deallocation distance' i.e. the travel distance
at which the AGV will deallocate the object.

## Properties

### agv

```flexscript
AGV AGV::TraversalPoint.agv (readonly)
```

The AGV that owns this traversal point.

### traversalDist

```flexscript
double AGV::TraversalPoint.traversalDist
```

The AGV travel distance at which the AGV will execute whatever logic is associated with this traversal point.

If this traversal point is an AGV.AllocationPoint,
this is the distance at which the allocated object will be deallocated.
You can set this property value, but only at defined times in the
AGV's travel operation. Specifically, you should only set it after the OnBuildTravelPath event is
fired (or as part of that trigger) but before (or as part of) the OnPreAllocate. Setting it
at any other
time is not guaranteed to work. Note that the AGV.traversalPoints
array is sorted in the order of the traversalDist value, so if you set
this value it may change the index of the traversal point within the AGV's traversalPoints
array, in order to preserve this ordering.
Since you have significant control over the deallocation timing using standard features like
Deallocation
Types, you generally would not need to change an allocation point's traversalDist. However,
in some special cases, such as when simulating AGVs with special spatial geometries, it may
be useful to change this property.

