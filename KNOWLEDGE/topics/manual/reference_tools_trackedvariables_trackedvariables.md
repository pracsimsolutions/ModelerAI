---
id: reference_tools_trackedvariables_trackedvariables
name: "Tracked Variables"
kind: manual
breadcrumb: ["Reference","Tools","TrackedVariables","TrackedVariables"]
tags: ["manual","reference","tools","trackedvariables"]
source: "manual/Reference/Tools/TrackedVariables/TrackedVariables.html"
---

# Tracked Variables

## Overview and Key Concepts

 

 

Tracked Variables are accessed from the Toolbox.

 

### Global Tracked Variables

 

Tracked Variables accessed through the toolbox are stored in the MODEL:/Tools/TrackedVariables folder. These tracked variables are global for the model. Additional Tracked Variables may be added to your model through this window.

 

### Local Tracked Variables

 

Tracked Variables can be stored on other nodes in the model. These include, but are not limited to, Global Tables and object labels. You can also create Tracked Variables dynamically using FlexScript.

 

 

 

## Properties

 

Tracked variables have the following properties:

 

| Icon | Description |
| --- | --- |
|  | Pins the tracked variable to a Dashboard as a table, bar, line or histogram graph. |

 

### Tracked Variable Drop Down

 

Shows the current Tracked Variables. Enter text to rename the Tracked Variable.

 

### Type

 

The type affects what metadata will be collected and how it will be calculated. Type can be one of the following values:

 

- **Level.** Records the variable as a level that can go up and down, such as content. The average is a time-weighted average.
- **Cumulative.** Records the variable as a value that only accumulates over time, such as input or output. Average is not tracked.
- **Time Series.** Records the variable as a series of independent values. The average is a non-time-weighted average. An example of this type is staytime tracking.
- **Categorical.** Records the variable as a set of values, where values do not have a mathematical relationship to each other. Average is not tracked. An example of this is state tracking.
- **Kinetic Level.** This is like the Level type, except that the level can also have a rate of change, such as for a battery level that is constantly depleting/recharging at a defined amperage, or a liquid level that increases or decreases based on a flow rate.

 

 

 

## Dashboard

 

Tracked variables can be displayed in the Dashboard in multiple graphs:

 

 

 

 

## Object Statistics using Tracked Variables

 

FlexSim's simulation objects also use tracked variables to record many of their own object-specific statistics. Each object will usually track the standard set of statistics, which are:

 

- Content
- Input
- Output
- State
- Staytime

 

Each object can also publish additional statistics depending on the type of object. To see the list of available statistics for an object, you can add a *Tracked Variable vs Time* chart to a dashboard, then use the button in its properties window to hover over the object of interest. This will list out the object's available statistics.

 

You can also manually get various statistic values of an object using the Object's `stats` property which will give you a reference to each Tracked Variable the object stores.

 

 

 

### Events

 

Using the process flow module's Wait For Event or Event-Triggered Source activities, you can also execute logic when an object's statistic changes. Use the button in the activity's properties pane and hover over the desired object to get a list of statistics that can be listened to. Statistic change events will have the format On[StatisticName]Change. A global or local tracked variable will have the OnChange event.

 

On Change occurs whenever the value of a tracked variable changes

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Old Value | double | The previous value of the tracked variable |
| New Value | double | The new value of the tracked variable |
