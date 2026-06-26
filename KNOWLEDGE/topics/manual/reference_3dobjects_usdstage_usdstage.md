---
id: reference_3dobjects_usdstage_usdstage
name: "USD Stage"
kind: manual
breadcrumb: ["Reference","3DObjects","USDStage","USDStage"]
tags: ["manual","reference","3dobjects","usdstage"]
source: "manual/Reference/3DObjects/USDStage/USDStage.html"
---

# USD Stage

## Overview and Key Concepts

 

The USD Stage class enables advanced USD workflows, such as:

 

- Connecting to Nvidia Omniverse usd stages
- Viewing prim hierarchy and properties
- Modifications of usd prim data
- Bi-directional collaborative editing of Nvidia Omniverse usd stages
- Adding FlexSim objects and data to imported stages
- Streaming simulation data updates for external rendering and video recording of animated simulation runs

  

#### Live Connection

 

When using the USD Stage object, a live connection is made to the specified usd stage. This connection is more than simply a one-time reading of the file. While connected, you can read and write changes to the file. The file must be available and connected for the USD Stage object to work.

 

The 3D representation of the usd stage is not stored with other imported media shapes in FlexSim. It is dynamically generated based on the information within the connected usd stage.

   

#### Limitations

 

The USD format is large and extensible. Similarly, FlexSim's 3D graphics engine is also dynamic and flexible. As such, certain features of FlexSim and USD may not translate perfectly during these workflows. Additional development may be required to support particular features of FlexSim and USD in certain workflows.

  

 

 

## Connecting to USD Stages

 

In the path field of the Stage Panel, browse to a usd file on disk or enter a destination path on an Omniverse Nucleus server.

 

e.g., omniverse://127.0.0.1/Users/UserName/Folder/FileName.usd

 

 

 

 

## Viewing Prim Hierarchy and Properties

 

After checking the Select Prims box on the Stage Panel, individual prims can be selected in the 3D view. Clicking on a prim in the 3D view will show the Prim Panel for that prim.

 

Press the Prim Path tree button to open a tree view focused on the selected prim. This view enables you to see the usd stage prim hierarchy and the properties of each prim.

 

 

 

 

## Modifying Prim Data

 

The Prim Panel enables modification of prim data.

 

To save modifications to prim data, press the Save button on the Stage Panel.

 

Press the Reload button to load any modifications to the stage made by other applications connected concurrently.

 

 

 

## Bi-directional Collaborative Editing

 

When connected to a usd stage on an Omniverse Nucleus server, the Omniverse Live Panel can be used to create or join a live session with other users.

 

While in a live session, changes you make to the stage will be sent to the other users, and changes made by other users will be received by FlexSim.

 

 

 

## Adding FlexSim Objects

 

The USD Stage object can be used as a container for other FlexSim objects. When a stage containing FlexSim objects is saved, those objects will be exported to usd format. When loading a stage with exported FlexSim objects, the prims representing those objects will be marked as invisible for the associated FlexSim objects within the USD Stage object.

 

 

 

## Streaming Simulation Updates

 

When connected to a USD Stage on an Omniverse Nucleus server, the Omniverse Live Panel can be used to create or join a live session with other users.

 

You can run the simulation model while in a live session. While the model is running, FlexSim objects and flowitems in the USD Stage object will stream transform updates to the other users in the session. In this way, you can record a running model from another application that also has live connectivity with Omniverse Nucleus usd stages.

 

Using USD's layering and composition features, you may be able to modify the stage to render using more photorealistic materials and shaders in another application rather than just using FlexSim's default preview surface materials.

  

#### Performance and Stability

 

The simulation streaming workflow is currently not being tested or developed. Due to NVIDIA's [deprecation](https://developer.nvidia.com/omniverse/legacy-tools) of the Omniverse Launcher, this workflow will remain unsupported until further notice.

  

 

 

## States

 

The USD Stage does not implement any automatic state logic.

 

 

 

## Properties

 

Below are the set of property names supported by USD Stages.

 

| Property | Type |
| --- | --- |
| DrawPlane | Boolean |
| DrawAxisLines | Boolean |
| PolygonOffsetFactor | Number |
| PolygonOffsetUnits | Number |
| SelectPrims | Boolean |
