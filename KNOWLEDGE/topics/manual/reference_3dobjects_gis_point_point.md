---
id: reference_3dobjects_gis_point_point
name: "Point"
kind: manual
breadcrumb: ["Reference","3DObjects","GIS","Point","Point"]
tags: ["manual","reference","3dobjects","gis"]
source: "manual/Reference/3DObjects/GIS/Point/Point.html"
---

# Point

## Overview and Key Concepts

 

Points are used to define locations in geographic coordinates. When connected, they create routes, which can be used by task executers to travel between points using the GIS Navigator.

 

 

The actual point objects are hidden within the GIS Navigator. Map objects draw shapes representing these point objects. You modify the point's properties by clicking on its shape on a map. A point may be drawn by zero, one, or more maps.

 

A point's size and rotation are used when drawn by a map. A point's cartesian location is not used when drawn by a map. Instead, it is positioned based on its geographic latitude and longitude properties.

 

 

 

## States

 

The point does not implement states.

 

 

 

## Statistics

 

The point does not track any statistics.

 

 

 

## Properties Panels

 

The Point object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Point
- Triggers

 

 

 

## Properties

 

The Point object uses the following properties:

 

| Property | Type |
| --- | --- |
| DrawPoint | Boolean |
| Latitude | Number |
| Longitude | Number |
