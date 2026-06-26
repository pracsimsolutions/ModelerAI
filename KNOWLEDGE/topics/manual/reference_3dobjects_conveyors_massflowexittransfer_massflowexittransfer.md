---
id: reference_3dobjects_conveyors_massflowexittransfer_massflowexittransfer
name: "Mass Flow Exit Transfers"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","MassFlowExitTransfer","MassFlowExitTransfer"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/MassFlowExitTransfer/MassFlowExitTransfer.html"
---

# Mass Flow Exit Transfers

## Overview and Key Concepts

 

A mass flow exit transfer represents a conversion point when translating mass flow units into flow items. It is similar to regular exit transfers in that it allows you to connect the mass flow conveyor to fixed resources, outputting flow items from the conveyor. In the mass flow case, though, it is performing a conversion calculation from mass flow units to each flow item. As different flow rates for units reach the end of the conveyor, it will use the Units per Package property in combination with the flow rate to determine an inter-arrival rate for flow items, and will create individual items at that rate. If downstream objects backup and cannot receive the generated items, the mass flow conveyor will begin to accumulate.

 

 

 

## Properties Panels

 

The Mass Flow Exit Transfer uses the following properties panels:

 

- Statistics
- Labels
- Mass Flow Exit Transfer
- Ports

 

 

 

## Properties

 

The Mass Flow Exit Transfer object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Mass Flow Exit Transfer object uses the following properties:

 

| Property | Type |
| --- | --- |
| PackageItem | Number |
| UnitsPerPackage | Number |
