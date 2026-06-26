---
id: reference_3dobjects_taskexecuters_transporter_transporter
name: "Transporter"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","Transporter","Transporter"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/Transporter/Transporter.html"
---

# Transporter

## Overview and Key Concepts

 

The transporter is used mainly to carry flow items from one object to another. It has a fork lift that will raise to the position of a flow item if it is picking up or dropping off to a rack. It can also carry several flow items at a time if needed.

 

 

The transporter is a task executer. It implements offset travel in two ways. First, if there is an involved flow item for the travel operation, then it travels so that the front of the fork lift is positioned at the destination x/y location and raises its fork to the z height of the destination location. Second, if there is no involved flow item for the offset travel operation, then it travels so that its x/y center and z base arrive at the destination location.

 

 

 

## Events

 

The transporter uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

This object uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The transporter uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Transporter object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Transporter
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Transporter object shares several properties with all task executers:

 

- Task Executer Properties
