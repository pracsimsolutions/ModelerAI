---
id: reference_tools_performancemeasuretables_performancemeasuretables
name: "Performance Measure Tables"
kind: manual
breadcrumb: ["Reference","Tools","PerformanceMeasureTables","PerformanceMeasureTables"]
tags: ["manual","reference","tools","performancemeasuretables"]
source: "manual/Reference/Tools/PerformanceMeasureTables/PerformanceMeasureTables.html"
---

# Performance Measure Tables

## Overview and Key Concepts

 

A performance measure table allows you to specify the performance measures for your model:

 

 

Performance Measure Tables are accessed from the Toolbox. For more information on how to use performance measures, see the Performance Measures topic.

 

 

 

## Performance Measure Table Properties

 

| Icon | Description |
| --- | --- |
|  | Duplicate the selected performance measure(s). |
|  | Remove the selected performance measure(s). |
|  | Move the selected performance measure(s) up or down in the list. |

 

### Performance Measures Row Edit

 

You can use this edit field or the spinner to adjust the number of performance measures in the table.

 

### Table Rows

 

Each row of the table allows you to edit the following attributes of each performance measure: 

- Name
- Value
- Display Units
- Description

 The name of each performance measure must be unique.

 

You cannot change the value of a performance measure directly. Instead, you'll specify an expression that determines the value of the performance measure.

 

The Display Units column can contain arbitrary text. However, that text should be fairly short, and should indicate the units of the parameter. It is also a helpful hint to the model user about the units of the parameter.

 

The Description column can contain arbitrary text. This text is usually a short phrase that indicates what the performance measure reports.

 

 

 

## Value Properties

 

When you select a cell in the second column of the performance measure table, a button will appear. Clicking that button will open the Value Properties for the selected value.

 

 

### Reference

 

Specifies a node related to this value. The reference is passed into the callback function for this value.

 

### Value

 

Specifies a function that is called whenever the value of this performance measure is required. The reference is passed in to this function.

 

In some advanced and unusual cases, you may need to return an arbitrary node structure from each replication of an experiment or optimization. If the repData node passed in to the function is present, you can add any structure you need to that node.

 

### Apply to All

 

Visible only if multiple values are selected. Applies any changes to all selected values, rather than just the first selected value.
