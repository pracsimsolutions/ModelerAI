---
id: reference_3dobjects_gis_map_map
name: "Map"
kind: manual
breadcrumb: ["Reference","3DObjects","GIS","Map","Map"]
tags: ["manual","reference","3dobjects","gis"]
source: "manual/Reference/3DObjects/GIS/Map/Map.html"
---

# Map

## Overview and Key Concepts

 

Maps are used to show geographic information in latitude and longitude coordinates. They draw flat textured tiles using a Mercator projection. Map tile textures are downloaded from a tile server using the URL specified on the GIS Navigator.

 

 

GIS points, routes, and travelers are drawn by maps. A model may contain multiple maps that may draw zero, one, or more representations of points, routes, and travelers in the model. The maps themselves do not contain the other objects; they simply draw lines and shapes representing the location of other objects in geographic coordinates.

 

 

 

## States

 

The map does not implement any states.

 

 

 

## Statistics

 

The map does not track any statistics.

 

 

 

## Properties Panels

 

The Map object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Map
- Ports
- Triggers

 

 

 

## Properties

 

The Map object uses the following properties:

 

| Property | Type |
| --- | --- |
| EditMode | Boolean |
| IgnoreObjects | Boolean |
| Latitude | Number |
| Longitude | Number |
| Zoom | Number |
| DrawBorder | Options |
| ScaleFactor | Number |
| MinScale | Number |
| MaxScale | Number |
