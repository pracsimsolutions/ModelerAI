---
id: reference_tools_gis_gis
name: "GIS Navigator"
kind: manual
breadcrumb: ["Reference","Tools","GIS","GIS"]
tags: ["manual","reference","tools","gis"]
source: "manual/Reference/Tools/GIS/GIS.html"
---

# GIS Navigator

## Overview and Key Concepts

 

The GIS Navigator is used to calculate the geographic position of travelers as they travel along routes between points on maps. The GIS Navigator has settings that affect how map tiles are rendered and how routes are calculated.

 

 

 

## Properties

 

The GIS Navigator has the following properties:

 

 

### Tile Server URL

 

Refer to the Maptiler platform guide for instructions on downloading raster map tiles.

 

This setting is also compatible with other third-party APIs that use the same tile request URL syntax.

 

Maptiler : Setup Guide

  

#### Usage Policies

 

Ensure that you consult and follow the usage policies of the specified server.

  <!--  

#### FlexSim Tile Server Usage Policy

 

The FlexSim server usage is restricted to reasonable use-cases from within the FlexSim software. We provide no guarantees with regard to uptime, latency, or data updates.

  --> <!-- 

Map data © OpenStreetMap contributors.

 --> 

### Tile Data Directory

 

Tile files are saved to this directory.

 

Delete existing files in this directory to download new versions of those files.

 

### Tile File Prefix

 

Specify an optional prefix to be added to tile filenames when saved on the hard drive.

 

Use different prefixes to distinguish between tiles when using different tile styles from different tile servers.

 

### Default Route Type

 

Define the default type for newly created routes.

 

### Air Route Calculation Average Speed

 

When calculating straight rhumb line and flight paths, use this average speed to calculate the route duration.

 

### Air Route Calculation Cost By

 

When calculating straight rhumb line and flight paths, use the calculated duration (Fastest Route) or distance (Shortest Route) for the route cost.

  

#### Air Route Calculations

 

These settings are used when air routes are updated. Existing routes will not be affected by simply changing these values. To update existing routes after changing these values, select multiple routes in red on a map and then press the Update button in the Route Properties panel.

  

### Routing Server URLs

 

Specify the URLs for [OSRM servers](http://project-osrm.org/) to download routing information. In addition to the Driving Roads Route Type, you can add more route types and URLs for other routing servers.

 

e.g., http://router.project-osrm.org/route/v1/driving/

  

#### Usage Policies

 

Ensure that you consult and follow the usage policies of the specified server.

 

e.g., [https://github.com/Project-OSRM/osrm-backend/wiki/Demo-server](https://github.com/Project-OSRM/osrm-backend/wiki/Demo-server)

  <!--  

#### FlexSim Routing Server Usage Policy

 

The FlexSim server usage is restricted to reasonable use-cases from within the FlexSim software. We provide no guarantees with regard to uptime, latency, or data updates.

  --> 

### Clear Routing Cache

 

Delete all cached routing server responses in the GIS Navigator.
