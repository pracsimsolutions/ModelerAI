---
id: reference_dashboard_charts_scenariochart_scenariochart
name: "Scenario Chart"
kind: manual
breadcrumb: ["Reference","Dashboard","Charts","ScenarioChart","ScenarioChart"]
tags: ["manual","reference","dashboard","charts"]
source: "manual/Reference/Dashboard/Charts/ScenarioChart/ScenarioChart.html"
---

# Scenario Chart

## Overview and Key Concepts

 

The Scenario Chart allows you to compare data from another chart across multiple Experiment scenarios (or Optimizer solutions) on a single chart.

 

 

 

To use the Scenario Chart, you must specify a target chart. The Scenario Chart will mimic the type of the target chart; if the target chart is a Bar Chart, the Scenario Chart will also be a Bar Chart. The Scenario Chart is compatible any of the following chart types: 

- Table Chart
- Bar Chart
- Pie Chart
- Histogram
- Box Plot
- Time Plot.

 In addition to specifying a target chart, you must also specify which scenarios you want to compare. The Scenario Chart uses this information to produce a chart that looks similar to the original chart, and also compares the requested scenarios. Since the Experimenter usually runs multiple replications per scenario, the Scenario Chart will aggregate data from all replications of each scenario. This way, the Scenario Chart will form a fair visual representation of the target chart's information for each scenario.

  

#### Sorting the Scenario Chart

 

The Scenario Chart adds a sort element to sort by scenario. In addition, when you select a target chart, any sort elements in that chart are added to the Scenario Chart, so that it will be sorted the same way.

   

#### Using the Scenario Chart with the Optimizer

 

The Scenario Chart can also compare solutions from an optimization.

   

#### Save output tables and charts

 

In order to use the Scenario Chart, be sure have the Experimenter save output tables and charts for each replication. For more information, see the Experimenter topic.

  

 

 

## Replication Aggregation

 

The Scenario Chart will aggregate data from all replications of each chosen scenario. Depending on the target chart's type, data may be aggregated quite differently. The following sections describe how the aggregation works in general.

 

### Table Charts

 

If the Scenario Chart is based on a table chart, you will need to specify which columns of the table to aggregate, and how they should be aggregated. All other columns are considered identifier columns, and are used to determine which rows should be aggregated together.

 

For example, suppose you had a table chart that showed the content of several queues. Then suppose you ran an experiment with two scenarios, and three replications per scenario. Each replication would produce a table chart, like the ones shown in the following image:

 

 

If you chose to compare Scenario 1 and Scenario 2, and to aggregate the WIP column, the Scenario Chart would look like the following:

 

  

#### Row Order and Aggregation

 

Rows from all replication data are first grouped by scenario. Then they are grouped by all non-aggregated columns. The aggregated values are determined for each of these groups. This means that the order of data in each replication doesn't affect the final aggregated values.

  

### Pie and Bar Charts

 

If the Scenario Chart is based on a Pie or Bar chart, then the Scenario chart will calculate the average value of each bar or pie segment across all replications of each scenario. Those averaged values will then be used to create an average version of each bar group or pie ring for each scenario.

 

For both charts, you may need to specify whether the text shown on the chart needs to be aggregated also. For example, a state pie chart might show a utilization percentage in the center. If text is present on each bar group or pie ring, but that text is not aggregated, then the Scenario Chart will use that text to identify which bar or segment values should be grouped together for aggregation.

 

### Box Plots and Histograms

 

If the Scenario Chart is based on a Box Plot or Histogram, then the resulting chart will be a box plot or histogram of all data from all replications, for each scenario.

 

### Time Plots

 

If the Scenario Chart is based on a Time Plot, you can specify an interpolation mode. If the interpolation mode is None, then all data from all replications will be drawn on the plot, colored by Scenario. Otherwise, you must addtionally specify a sample rate and a confidence interval. The Scenario Chart will create time intervals, based on the sample rate, and sample the data at those intervals. For each replication, it is likely that a data point was not recorded at the interval time, so the Scenario Chart will interpolate to find the value. Those interpolated values are then aggregated to determine the mean. In addition, you can also show the min, max and confidence interval values.

  

#### Interpolation Assumes Ordered Data

 

If you use an interpolation mode, the Scenario Chart assumes the replication data for the target chart is ordered by its x-axis values from least to greatest. If this is not true, the values may be innacurate. For time plots, data is usually gathered chronologically, so this assumption usually holds.

  

 

 

## Properties Panels

 

The scenario chart uses the following properties panels:

 

- Data

 

Other panels will appear in the properties window. However, the other panels deal with the settings specific to the specified chart.
