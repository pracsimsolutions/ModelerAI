---
id: using3dobjects_universalscenedescription_universalscenedescription
name: "Universal Scene Description (USD)"
kind: manual
breadcrumb: ["Using3DObjects","UniversalSceneDescription","UniversalSceneDescription"]
tags: ["manual","using3dobjects","universalscenedescription","universalscenedescription"]
source: "manual/Using3DObjects/UniversalSceneDescription/UniversalSceneDescription.html"
---

# Universal Scene Description (USD)

## Introduction

 

Pixar's [Universal Scene Description](https://openusd.org/) (USD) format is a set of software modules that enable robust interchange of data between digital content creation tools. FlexSim has support for USD in the following features:

 

- 3D shape importer
- Model exporter
- USD Stage object

 

FlexSim's USD features also provide interactions with [Nvidia's Omniverse](https://www.nvidia.com/omniverse/), which is a computing platform that enables USD-based 3D workflows and collaboration.

 

 

 

## 3D Shape Importer

 

USD files can be imported as an object's shape using the Visuals Properties panel.

 

USD is a large, extensible format with many features that are not used by the basic 3D shape importer. When importing a usd file as an object's shape, only geometric 3D mesh data and basic surface material data will be loaded from the file. Other data within the usd file will be ignored. Importing USD animation data is not supported at this time.

  

#### Layered USD Files - Embedded Media

 

The USD format includes support for referencing external usd files within a usd file. When loading a stage with a complex composition, FlexSim's packed media feature only embeds the main usd file and referenced textures. Externally referenced usd files will not be embedded into the fsm file.

 

To ensure that referenced usd files are visible when sharing a FlexSim model, the references should be relative paths and the referenced files should be distributed along with the FlexSim model file.

 

Alternatively, usd files can be "flattened" to embed any external files into the main file to remove layered composition. Internal usd prim references could also be used.

  

 

 

## Model Exporter

 

The current state of a FlexSim model can be exported to USD. For more information, see the Omniverse Connector tool.

 

 

 

## USD Stage Object

 

In addition to basic importing and exporting of usd files, FlexSim supports more complex USD workflows via the USD Stage object. These workflows include:

 

- Connecting to Nvidia Omniverse usd stages
- Viewing prim hierarchy and properties
- Modifications of usd prim data
- Bi-directional collaborative editing of Nvidia Omniverse usd stages
- Adding FlexSim objects and data to imported stages
- Streaming simulation data updates for external rendering and video recording of animated simulation runs
