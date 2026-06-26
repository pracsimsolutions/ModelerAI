---
id: reference_tools_omniverseconnector_omniverseconnector
name: "Omniverse Connector"
kind: manual
breadcrumb: ["Reference","Tools","OmniverseConnector","OmniverseConnector"]
tags: ["manual","reference","tools","omniverseconnector"]
source: "manual/Reference/Tools/OmniverseConnector/OmniverseConnector.html"
---

# Omniverse Connector

## Overview and Key Concepts

 

The Omniverse Connector is used to export Universal Scene Description (USD) files.

 

You can export directly to files on disk or to an Omniverse Nucleus server.

 

You can export 3D geometric data, object property metadata, or both.

 

Only 3D mesh geometric data is exported. Imported AutoCAD drawings (dwg files) are not exported.

 

Only basic surface material information is exported: colors, diffuse textures, and normalmap textures. Advanced material configurations should be applied using external software after importing the usd file exported from FlexSim.

  

#### USD Animation

 

The USD export doesn’t currently export object movement. It exports a snapshot the state of the model at the time you press the Export button.

 

The animation data stored on operators puts their shape into the appropriate pose at that particular moment in time.

 

If you run the animation of the usd file, then operators will animate to different poses in their animations, but nothing about the creation of objects or movement of objects from a running simulation is encoded into the usd file, only the current state at the time you press Export.

 

For advanced workflows including live connectivity, use a USD Stage object.

  

 

 

## Properties

 

The Omniverse Connector has the following properties:

 

 

### File Path

 

Browse to an output file path on disk or enter a destination path on an Omniverse Nucleus server (e.g., omniverse://127.0.0.1/Users/UserName/Folder/FileName.usd).

 

USD supports three file formats:

 

| Extension | Content |
| --- | --- |
| .usda | Human-readable UTF-8 text |
| .usdc | Random-access “Crate” binary |
| .usd | Either of the above |

 

In general when exporting to usd, a binary file will be created, but usda files can be renamed to have an extension of usd as well.

 

### Container

 

This field is left blank by default to export the entire model. To export a subset of your model, you can specify a container.

 

### Export Meshes

 

Clear this checkbox to not export mesh data into the usd file. This enables you to export a hierarchy of objects with properties but without their 3D geometry.

 

### Export Camera

 

Clear this checkbox to not export the 3D view's position as a camera in the usd file.

 

### Copy Textures to Output Directory

 

Textures are referenced by path within usd files. They are not embedded into the usd file itself. By default, FlexSim will export the paths to where textures were loaded from disk (e.g., C:/Program Files/FlexSim/fs3d/Processor/ProTex.png). These paths may be absolute or relative paths. When transferring the usd file to another computer, if the textures aren't found in the same location, then they will not be displayed.

 

Checking this box will copy textures from their original positions to the output directory of the usd file, and the references within the usd file will be relative paths to those copied textures from the output usd file. This makes the textures easier to distribute with the output usd file.

 

Only textures used by exported meshes will be copied.

 

If a texture already exists at the target location, it will be skipped, not overwritten. Delete destination texture files before exporting if you want to re-copy them.

 

### Subdirectory

 

Copy textures to the specified subdirectory of the output directory.

 

Clear this field to copy textures directly to the output directory (the same folder as the usd file).

 

### Export Object Properties

 

Specify a set of object property tables to export as attributes of the usd prims.

 

The set of objects and properties in each table will be exported to the usd file.

 

### Export

 

Press this button to perform the export. A 3D view must be open in the model to perform the export because it may include meshes dynamically drawn via code.
