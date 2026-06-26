---
id: reference_3dobjects_conveyors_exittransfers_exittransfers
name: "Exit Transfers"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","ExitTransfers","ExitTransfers"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/ExitTransfers/ExitTransfers.html"
---

# Exit Transfers

## Overview and Key Concepts

 

Exits Transfers are connections from a conveyor to a non-conveyor object (such as a Sink or other Fixed Resource) to a conveyor object. They are represented by a white box on a conveyor in the simulation model. Exit transfer Be aware that Exit transfers are different from transfers (which connect conveyors to other conveyors) and entry transfers (which are connections from a non-conveyor object to a conveyor).Exit Transfers have the ability to send for a transport, or in other words, a Task Executer to pick up items and deliver them to another object. Task Executers can pick up items from a particular point on the Exit Transfer or a range of possible points along an Exit Transfer.

 

 

Exit transfers are not available in the FlexSim Library. Rather, they are created whenever a conveyor is connected to another object.

 

 

 

## Properties Panels

 

The Exit Transfer uses the following properties panels:

 

- Statistics
- Labels
- Exit Transfer
- Output
- Ports

 

 

 

## Properties

 

The Exit Transfer object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Exit Transfer object uses the following properties:

 

| Property | Type |
| --- | --- |
| ContinuousPickPrediction | Boolean |
| IgnoreOnPredictLatePickup | Boolean |
| PickPredictionInterval | Unit |
| StopAtEnd | Boolean |
