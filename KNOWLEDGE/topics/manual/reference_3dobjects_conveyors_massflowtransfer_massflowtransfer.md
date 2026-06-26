---
id: reference_3dobjects_conveyors_massflowtransfer_massflowtransfer
name: "Mass Flow Transfers"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","MassFlowTransfer","MassFlowTransfer"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/MassFlowTransfer/MassFlowTransfer.html"
---

# Mass Flow Transfers

## Overview and Key Concepts

 

A mass flow transfer is created when you make an 'A' connection between two Mass Flow Conveyors. By default, mass flow transfers will cause the conveyors to behave as-if they were snapped together directly. In other words, if you create a mass flow transfer between two conveyors, the upstream conveyor will perform its flow distribution algorithm as if the downstream conveyor were attached directly to its end.

 

You would use a mass flow transfer for the following reasons:

 

- The layout of various objects in the model makes it difficult or burdensome to snap the conveyors together directly.
- You want flow between the conveyors to convert from one unit to a different unit.

 

 

 

## Properties Panels

 

The Mass Flow Transfer uses the following properties panels:

 

- Mass Flow Transfer - only shown on downstream transfers
- Labels
- Ports

 

 

 

## Properties

 

The Mass Flow Transfer object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, downstream Mass Flow Transfer objects include the following properties:

 

| Property | Type |
| --- | --- |
| ConvertToUnit | Code |
| UnitsPerPackage | Number |
