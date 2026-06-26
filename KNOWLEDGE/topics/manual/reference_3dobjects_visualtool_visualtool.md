---
id: reference_3dobjects_visualtool_visualtool
name: "Visual Tool"
kind: manual
breadcrumb: ["Reference","3DObjects","VisualTool","VisualTool"]
tags: ["manual","reference","3dobjects","visualtool"]
source: "manual/Reference/3DObjects/VisualTool/VisualTool.html"
---

# Visual Tool

## Overview and Key Concepts

 

The Visual Tool class enables several 3D visualization capabilities. While you won't see this class directly in the library, several of the objects in the library's **Visual** group are instances of this class, including the **Text**, **Billboard**, **Plane**,**Slide**, and **Shape** objects.

 

Visual tools can be 3D "props" in your model. They can also display text information, etc. See Using Visual Objects for more information on how you can use visual tools.

 

 

 

## States

 

The visual tool does not implement any automatic state logic.

 

 

 

## Properties

 

Below are the set of property names supported by visual tools.

 

| Property | Type |
| --- | --- |
| BillboardMode | Options |
| Divisions | Number |
| InObjectsInternal | Array of strings |
| Luminous | Boolean |
| OutObjectsInternal | Array of strings |
| PassInputsTo | String |
| PassOutputsTo | String |
| PolygonOffsetFactor | Number |
| PolygonOffsetUnits | Number |
| TextDisplay | Code String |
| TextSize | Unit |
| TextThickness | Unit |
| Texture | String |
| TextureXRepeat | Number |
| TextureYRepeat | Number |
| VisualDisplay | Options |
