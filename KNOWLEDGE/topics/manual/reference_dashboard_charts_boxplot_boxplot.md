---
id: reference_dashboard_charts_boxplot_boxplot
name: "Box Plot"
kind: manual
breadcrumb: ["Reference","Dashboard","Charts","BoxPlot","BoxPlot"]
tags: ["manual","reference","dashboard","charts"]
source: "manual/Reference/Dashboard/Charts/BoxPlot/BoxPlot.html"
---

# Box Plot

## Overview and Key Concepts

 

The Box Plot allows you to make boxplots (also called box-and-whisker plots) using data from a Statistics Collector:

 

 

### Data Format

 

At a minimum, the box plot requires a table with a single column, where each row contains a value that should counted in one of the boxes. However, you can use additional columns to categorize each data point by color and/or by axis.

 

For example, the table below has three columns:

 

 

The data used to create the box plot is found in the Duration column. The other two columns are used to categorize each point. The chart shown at the beginning of this section categorized points first by the value in the Object column, then by the value in the Type column. Each unique Object column value adds an additional x axis category, and each unique Type value adds a unqiue color.

 

Given the table shown above, you could use the Type column to determine the axis, and the Queue column to determine the color. The resulting chart might look like the following:

 

 

Alternatively, you could choose to ignore any categorizing values, to get a feel for the overall distribution. If you used the previous table to make a box plot without categorizing any points, it might look like the following:

 

 

### Calculations

 

For the lines that form the center box, they are drawn at the first quartile, the median, and the third quartile. The whiskers are drawn to the first data point that is less than 1.5 * IQR (inter-quartile range) away from either the first or third quartiles.

 

The mean and the confidence intervals (if shown) are calculated using all data, including data outside the whisker range.

 

 

 

## Performance Measures

 

The Box Plot provides performance measures that aggregate the data in the column specified by the Value Column property. The chart will always provide the Average, Minimum, or Maximum of that column as a performance measure. If the either the Axis Categories or the Color Categories columns are specified, the chart will allow you to obtain the Average, Minimum, or Maximum of the value column, filtering by a specific category.

 

 

 

## Properties Panels

 

The Box Plot uses the following properties panels:

 

- Data
- Settings
- Colors
- Sorting
