---
id: reference_propertiespanels_gispanels_map_map
name: "Map"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","GISPanels","Map","Map"]
tags: ["manual","reference","propertiespanels","gispanels"]
source: "manual/Reference/PropertiesPanels/GISPanels/Map/Map.html"
---

# Map

The Map panel has options for controlling the Map's properties.

 

 

The following properties are on the Map panel:

 

### Edit Mode

 

Edit Mode enables the map to be panned and zoomed in the 3D view. If cleared, then dragging the map tile textures will pan the view instead of the map.

 

This only affects the map tile textures. The border of the map is not affected by this property. Neither are points, routes, or travelers drawn by the map. Use the Protected flag on points to prevent them from being moved.

 

### Ignore Objects

 

If this box is checked, the user will not be able to click on any objects in the map view window. This is useful for navigating around a map that is completed, as the user will not be able to accidentally move any objects.

 

### Search URL

 

Add a configurable search endpoint that allows you to provide a custom location search URL (for example, `https://nominatim.openstreetmap.org/search.php?q=${q}&format=json`).

 

When you enter a location in the search field, the application queries this URL by replacing `${q}` with your input.

 

The response is used to obtain latitude and longitude, and the map center and zoom level are updated automatically based on the result.

 

### Search

 

Query the geocoding server for a location by name or address and set the map's latitude, longitude, and zoom based on the result.

 

### Latitude

 

This number defines the latitude of the center of the map.

 

### Longitude

 

This number defines the longitude of the center of the map.

 

### Zoom

 

This number defines the zoom level of the map. Higher numbers are zoomed in.

 

### Draw Border

 

This number defines how the border is drawn. If no border is drawn, the map can still be selected by clicking an edge.

 

The size of the thick frame border is based on the Z size of the map. The scale of tile textures is also affected by the map's size. If you want to adjust the relative size of the thick frame border, first change the Z size of the map object and then adjust the zoom back to the desired level.

 

### Scale Factor

 

This number defines an additional custom scale factor to be used when drawing traveler and point shapes. The zoom level of the view, map size, object size, and model units are also used to scale the size of traveler and point shapes.

 

### Min Scale

 

This number defines the minimum scale value at which traveler and point shapes will stop being scaled down as the view is zoomed in.

 

Specify 0 to not use a minimum scale.

 

Use the << button to get the scale value of the map according to the view's current zoom level.

 

### Max Scale

 

This number defines the maximum scale value at which traveler and point shapes will stop being scaled up as the view is zoomed out.

 

Specify 0 to not use a maximum scale.

 

Use the << button to get the scale value of the map according to the view's current zoom level.
