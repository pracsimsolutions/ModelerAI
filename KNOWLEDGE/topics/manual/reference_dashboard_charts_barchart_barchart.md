---
id: reference_dashboard_charts_barchart_barchart
name: "Bar Chart"
kind: manual
breadcrumb: ["Reference","Dashboard","Charts","BarChart","BarChart"]
tags: ["manual","reference","dashboard","charts"]
source: "manual/Reference/Dashboard/Charts/BarChart/BarChart.html"
---

# Bar Chart

## Overview and Key Concepts

 

The Bar chart can be used to visualize and compare similar values from many objects. The example below shows the time in state for several objects.

 

 

There are two possible formats for the data table for the bar chart. The are described in the following sections.

 

### One Bar Group per Row

 

This is the simpler format. The bar chart assumes that each row in the table represents a group of bars. You can specify which columns to draw individual bars for. In addition, you can specify up to to columns to display to the left of the bars. Here is an example table:

 

 

With this data format and table, you can specify the following settings:

 

 

With those settings, the resulting chart would be similar to the following:

 

 

### One Bar per Row

 

This format is more flexible. In this format, the bar chart assumes that each row represents a single bar in the bar group. You can specify which column determines the bar's size, which column determines the bar's color, and which column(s) determine how the bars are grouped. Here is an example table:

 

 

With this data format and table, you can specify the following settings:

 

 

With those settings, the resulting chart would be similar to the following:

 

 

You could alternatively use the following settings, where the Bar Color and Split By options are switched:

 

 

This switch in settings creates a chart like the following:

 

 

 

 

## Performance Measures

 

The Bar Chart provides the following performance measures:

 

If the Bar Label is set to a numeric column, then the chart will provide the Average, Sum, Minimum, or Maximum of that column as a performance measure.

 

For each included column, the chart will provide the Average, Sum, Minimum, or Maximum of the column as a performance measure.

 

 

 

## Properties Panels

 

The Bar Chart uses the following properties panels:

 

- Data
- Settings
- Text
- Colors
- Sorting
