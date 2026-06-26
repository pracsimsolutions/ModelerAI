---
id: reference_3dobjects_conveyors_massflowentrytransfer_massflowentrytransfer
name: "Mass Flow Entry Transfers"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","MassFlowEntryTransfer","MassFlowEntryTransfer"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/MassFlowEntryTransfer/MassFlowEntryTransfer.html"
---

# Mass Flow Entry Transfers

## Overview and Key Concepts

 

A mass flow entry transfer represents a conversion point when translating flow items into mass flow units. It is similar to regular entry transfers in that it allows you to connect fixed resources to the conveyor, sending items to it. In the mass flow case, though, when items enter a mass flow entry transfer, each item is translated into a quantity of mass flow units and then destroyed. The mass flow conveyor will create a "burst" of mass flow units, receiving those units using its full convey capacity (determined by width and speed). Once the quantity of units has all flowed onto the conveyor, the conveyor's entry flow rate will then go back to zero, and the mass flow entry transfer will become available to receive the next item.

 

 

 

## Properties Panels

 

The Mass Flow Entry Transfer uses the following properties panels:

 

- Statistics
- Labels
- Mass Flow Entry Transfer
- Ports

 

 

 

## Properties

 

The Mass Flow Entry Transfer object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Mass Flow Entry Transfer object uses the following properties:

 

| Property | Type |
| --- | --- |
| FlowUnit | Number |
| UnitsPerItem | Number |
