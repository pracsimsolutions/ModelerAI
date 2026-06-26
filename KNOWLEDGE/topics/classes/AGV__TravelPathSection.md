---
id: agv__travelpathsection
name: "AGV::TravelPathSection"
kind: class
module: "AGV"
signature: "AGV::TravelPathSection"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "A structure representing a traversal of a specific section of path during an AGV's travel operation."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.TravelPathSection.xml"
---

# AGV::TravelPathSection

A structure representing a traversal of a specific section of path during an AGV's travel operation.

When an AGV starts a travel operation, it builds an internal representation
of the path that it will travel on in getting to its destination. The path is stored in
AGV.travelPath, which is an
array-like structure with a length property and an array-dereference
(bracket: []) operator. Each accessed element is an instance of AGV.TravelPathSection. It
represents a sub-section of an AGV path that will be traversed, and contains the properties
described here. You can also access the travel path section that an AGV is currently traveling
on with AGV.currentTravelPathSection

//Printing an AGV's full path
AGV agv = AGV(current);
for (int i = 1; i <= agv.travelPath.length; i++) {
AGV.TravelPathSection sec = agv.travelPath[i];
print("path ", sec.path,
" fromDist ", sec.fromDist,
" toDist ", sec.toDist,
" fromTravelDist ", sec.fromTravelDist);
}

## Properties

### fromDist

```flexscript
double AGV::TravelPathSection.fromDist (readonly)
```

The distance along the path that is the starting point of the AGV's traversal on that path.

### fromTravelDist

```flexscript
double AGV::TravelPathSection.fromTravelDist (readonly)
```

The AGV travel distance that coincides with the starting point of the AGV's traversal on the path.

The AGV tracks its own cumulative travel distance across each of the
sections in its travel path. Each travel operation starts at a travel distance of 0,
and travel distance increases as it travels along the path. You can access the AGV's
current travel distance at a given time in the travel with
AGV.currentTravelDist.

### isAGVOritentedPathForward

```flexscript
bool AGV::TravelPathSection.isAGVOritentedPathForward (readonly)
```

True if, while traveling along this section, the AGV is oriented forward along the path, i.e. the AGV's 'head' is directed forward along the path's 'primary' direction.

### isAGVTravelOrientedForward

```flexscript
bool AGV::TravelPathSection.isAGVTravelOrientedForward
```

True if, while traveling along this section, the AGV is traveling in a forward direction, i.e. its 'head' is leading in the direction of travel.

### isPathTravelOrientedForward

```flexscript
bool AGV::TravelPathSection.isPathTravelOrientedForward (readonly)
```

True if the section's travel direction is forward along the path, i.e. the AGV is traveling in the path's 'primary' direction.

### path

```flexscript
Object AGV::TravelPathSection.path (readonly)
```

The path that this section is associated with.

### toDist

```flexscript
double AGV::TravelPathSection.toDist (readonly)
```

The distance along the path that is the ending point of the AGV's traversal on that path.

