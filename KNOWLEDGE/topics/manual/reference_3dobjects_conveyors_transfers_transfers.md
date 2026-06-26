---
id: reference_3dobjects_conveyors_transfers_transfers
name: "Transfers"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","Transfers","Transfers"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/Transfers/Transfers.html"
---

# Transfers

## Overview and Key Concepts

 

Transfers are not available in the FlexSim Library. Rather, they are created whenever a conveyor is connected to another conveyor.

 

 

There are three kinds of transfers:

 

- **Transfers** - Connections from one conveyor to another. They can affect how an item is transferred between conveyors.
- **Entry Transfers** - Connections from a non-conveyor object (such as a Source or other Fixed Resource) to a conveyor object. They can affect how an item is transferred to a conveyor. See Entry Transfers for more information.
- **Exits Transfers** - Connections from a conveyor to a non-conveyor object (such as a Sink or other Fixed Resource) to a conveyor object. Exit Transfers have the ability to send for a transport, or in other words, a Task Executer to pick up items and deliver them to another object. Task Executers can pick up items from a particular point on the Exit Transfer or a range of possible points along an Exit Transfer. See Exit Transfers for more information.

 

 

 

## Properties Panels

 

The Conveyor Transfer uses the following properties panel:

 

- Conveyor Transfer

 

 

 

## Types

 

Conveyor transfers use Conveyor Transfer Types
