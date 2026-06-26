---
id: reference_peopleobjects_3dobjects_multilocation_multilocation
name: "MultiLocation"
kind: manual
breadcrumb: ["Reference","PeopleObjects","3DObjects","MultiLocation","MultiLocation"]
tags: ["manual","reference","peopleobjects","3dobjects"]
source: "manual/Reference/PeopleObjects/3DObjects/MultiLocation/MultiLocation.html"
---

# MultiLocation

## Overview and Key Concepts

 

The MultiLocation object is a specialized Location object which contains other Location objects. This allows the object to represent a waiting room, a dining table, a park bench, or any other high capacity object. Usually, only the individual locations inside the multilocation will be acquired and released. However, if the **Acquire As Single Unit** option is used, only the multilocation itself will be acquired and released.

 

 

 

 

## Events

 

The MultiLocation uses same events as a location object. See Location Events for an explanation of these events.

 

 

 

## States

 

The MultiLocation uses same states as a location object. See Location States for an explanation of these states.

 

 

 

## Statistics

 

The MultiLocation uses same statistics as a location object. See Location Statistics for an explanation of these statistics.

 

In addition the MultiLocation totals some of the statistics of the MultiLocation and SubLocations.

 

### Throughput Total

 

Input Total and Output total are the total Input and Output of the MultiLocation and SubLocations.

 

### Content Total

 

Content Total is the total Content of the MultiLocation and SubLocations.

 

 

 

## Properties Panels

 

The MultiLocation object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- MultiLocation
- Ports
- Triggers
