---
id: reference_3dobjects_warehousing_driveinrack_driveinrack
name: "Drive In Rack"
kind: manual
breadcrumb: ["Reference","3DObjects","Warehousing","DriveInRack","DriveInRack"]
tags: ["manual","reference","3dobjects","warehousing"]
source: "manual/Reference/3DObjects/Warehousing/DriveInRack/DriveInRack.html"
---

# Drive In Rack

## Overview and Key Concepts

 

The Drive In Rack is a subclass of the Rack, which is in turn a subclass of the Storage Object. Most logic for the Drive In Rack is implemented as part of the Storage Object class. The main specialization of the Drive In Rack is that it defines customized picking/placing motion for those loading/unloading the rack. Task executers must first drive parallel to the pick face until they reach the assigned bay. Then they drive straight into the rack's bay. Once finished loading/unloading, they must finally drive straight out of the bay before continuing to a next task.

 

 

 

## Events

 

See the Storage Object for information on events.

 

 

 

## States

 

The Drive In Rack does not implement any states. Use the content graph to get statistics.

 

 

 

## Statistics

 

See the Storage Object for information on statistics.

  

## Properties

 

See the Storage Object for information on properties panels.

 

 

 

## Properties

 

See the Storage Object for information on properties.
