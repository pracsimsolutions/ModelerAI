---
id: reference_3dobjects_conveyors_entrytransfers_entrytransfers
name: "Entry Transfers"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","EntryTransfers","EntryTransfers"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/EntryTransfers/EntryTransfers.html"
---

# Entry Transfers

## Overview and Key Concepts

 

Entry transfers are Connections from a non-conveyor object (such as a Source or other Fixed Resource) to a conveyor object. They can affect how an item is transferred to a conveyor. They are represented by a white box on a conveyor in the simulation model. Be aware that entry transfers are different from transfers, (which connect conveyors to other conveyors) and exit transfers (which are connections from a conveyor object to a non-conveyor object).

 

 

Entry transfers are not available in the FlexSim Library. Rather, they are created whenever a conveyor is connected to another object.

 

 

 

## Properties Panels

 

The Entry Transfer uses the following properties panels:

 

- Statistics
- Labels
- Entry Transfer
- Input
- Ports

 

 

 

## Properties

 

The Entry Transfer object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Entry Transfer object uses the following properties:

 

| Property | Type |
| --- | --- |
| EntryOrientation | Number |
| HoldTransports | Boolean |
| ItemInsertionMode | Options |
| MaxTransportsIn | Number |
