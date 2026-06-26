---
id: reference_3dobjects_warehousing_rack_rack
name: "Rack"
kind: manual
breadcrumb: ["Reference","3DObjects","Warehousing","Rack","Rack"]
tags: ["manual","reference","3dobjects","warehousing"]
source: "manual/Reference/3DObjects/Warehousing/Rack/Rack.html"
---

# Rack

## Overview and Key Concepts

 

 

The Rack is a subclass of the Storage Object. Most logic for the Rack is implemented as part of the Storage Object class. The main specialization of the Rack is that it defines bays in the X direction, and levels in the Z direction, in contrast to the Floor Storage class, which defines a grid of storage locations on the floor.

 

### Pick Face

 

All racks, including gravity flow racks, push back racks, and drive in racks, implement a pick face that is on their "Y-" side. This means that items are generally meant to be picked from the side of the rack that faces its local negative Y direction. When using default visualizations, a standard rack will render cell supports for the pick face as orange, whereas the "back" face will have cell supports that are gray. For gravity flow and push back racks, the slots will be tilted slightly toward the pick face, as items flow toward that face.

 

 

 

## Events

 

See the Storage Object for information on events.

 

 

 

## States

 

The Rack does not implement any states. Use the content graph to get statistics.

 

 

 

## Statistics

 

See the Storage Object for information on statistics.

  

## Properties

 

See the Storage Object for information on properties panels.

 

 

 

## Properties

 

The Rack shares several properties with the Storage Object:

 

- Storage Object Properties

 

Additionally, the Rack uses the following properties:

 

| Property | Type |
| --- | --- |
| ExtendColumns | Boolean |
| PickPlaceYOffset | Unit |
| ShelfTilt | Unit |
