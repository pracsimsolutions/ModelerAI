---
id: gettingdata_standarddatagathering_overviewcharttemplates_overviewcharttemplates
name: "Overview of Chart Templates"
kind: manual
breadcrumb: ["GettingData","StandardDataGathering","OverviewChartTemplates","OverviewChartTemplates"]
tags: ["manual","gettingdata","standarddatagathering","overviewcharttemplates"]
source: "manual/GettingData/StandardDataGathering/OverviewChartTemplates/OverviewChartTemplates.html"
---

# Overview of Chart Templates

## Introduction to Chart Templates

 

Chart templates are intended to help you create commonly needed statistics in a quick, easy, and user-friendly manner. You should select a chart based on what type of statistics you're interested in and how you want to visualize the data from that statistic. When you use a chart template, you'll be able to specify the objects from which you would like to gather statistics. When you drag one of the charts to the dashboard and set up its basic properties, the chart will auto-generate the necessary tools that are needed for that statistical task (statistics collectors, calculated tables, groups, etc.). However, these tools will remain hidden in the background unless you install them in your simulation model. See Working With Dashboards and Charts for more information.

 

The following sections will provide a high-level overview of all the chart templates that are currently available in FlexSim.

 

 

 

## Content Templates

 

Content chart templates can be used to graph the content or WIP (work-in-progress) of many individual objects, process flow activities, or a specified set of objects. In other words, these templates will tell you the content of the involved objects, such as the current, minimum, maximum and average number of flow items or tokens being contained by those objects.

 

The following table provides a brief overview of the available templates:

 

| Name and Icon | Description |
| --- | --- |
| Content (Table) | Creates a table chart that displays the current content (WIP) for all selected objects. See the Content Table Template reference page for more information. |
| Content (Bar) | Creates a bar chart in which the length of the bars is determined by the current content (WIP) for all selected objects. See the Content Bar Template reference page for more information. |
| Content Vs Time | Creates a stair-step time plot displaying the changes in the content (WIP) for all selected objects over time. See the Content Vs Time Template reference page for more information. |
| Average Content (Table) | Creates a table that shows the average content (WIP) for all selected objects. See the Average Content Table Template reference page for more information. |
| Average Content (Bar) | Creates a bar chart that shows the average content (WIP) for all selected objects. The length of the bars show the average content of the objects. See the Average Content Bar Template reference page for more information. |
| Composite WIP (Table) | Creates a table that displays the current content (WIP) for a group of selected objects. The chart will track how many flow items or tokens are between an entry object and an exit object. See the Composite WIP Table Template reference page for more information. |
| Composite WIP (Bar) | Creates a bar chart that displays the current content (WIP) for a group of selected objects. The length of the bars shows the content for the entire group of objects. The chart will track how many flow items or tokens are between an entry object and an exit object. See the Composite WIP Bar Template reference page for more information. |
| Composite WIP Vs Time | Creates a stair-step chart that displays the changes in the content (WIP) for a group of selected objects over time. The chart will track how many flow items or tokens are between an entry object and an exit object over time. See the Composite WIP Vs Time Template reference page for more information. |
| WIP By Type (Table) | Creates a table that breaks down the content (WIP) in an object based on a flow item or token's type or label. It can also track the content of composite objects, so the content between an entry object and an exit object. See the WIP By Type Table Template reference page for more information. |
| WIP By Type (Bar) | Creates a bar chart that breaks down the content (WIP) in an object based on a flow item or token's type or label. It can also track the content of composite objects, so the content between an entry object and an exit object. See the WIP By Type Bar Template reference page for more information. |
| WIP By Type Vs Time | Creates a stair-step chart that displays the changes over time in the content (WIP) in a object based on based on a flow item or token's type or label. It can also track the content of composite objects, so the content between an entry object and an exit object. See the Composite WIP Vs Time Template reference page for more information. |

 

 

 

## Output Templates

 

Output templates can be used to graph the output of many individual objects, process flow activities, or a specified set of objects. In other words, these templates will calculate the number of flow items or tokens that have left the object or activity.

 

The following table provides a brief overview of the available templates:

 

| Name and Icon | Description |
| --- | --- |
| Output (Table) | Creates a table that shows the output of the selected objects. See the Output Table Template reference page for more information. |
| Output (Bar) | Creates a bar chart in which the length of the bars is determined by the current output for all selected objects. See the Output Bar Template reference page for more information. |
| Output Per Hour (Table) | Creates a table that that shows the total output of the selected objects divided by the total number of hours the simulation has run so far. See the Output Per Hour Table Template reference page for more information. |
| Output Per Hour (Bar) | Creates a bar chart that shows the total output of the selected objects divided by the total number of hours the simulation has run so far. See the Output Per Hour Bar Template reference page for more information. |
| Output Per Hour (vs Time) | Creates a line graph that shows the changes over time in the output of the selected objects for each hour in the simulation run so far. See the Output Per Hour vs Time Template reference page for more information. |
| Composite Output (Table) | Creates a table that displays the combined output of all selected objects. See the Composite Output Table Template reference page for more information. |
| Composite Output (Bar) | Creates a bar chart that displays the combined output of all selected objects. See the Composite Output Bar Template reference page for more information. |
| Composite Output Per Hour (Table) | Creates a table that displays the combined output of all selected objects divided by the total number of hours in the simulation run so far. See the Composite Output Per Hour Table Template reference page for more information. |
| Composite Output Per Hour (Bar) | Creates a bar chart that displays the combined output of all selected objects divided by the total number of hours in the simulation run so far. See the Composite Output Per Hour Bar Template reference page for more information. |
| Composite Output Per Hour (Vs Time) | Creates a line graph that shows the changes over time in the combined throughout of all selected objects for each hour the simulation has run so far. See the Composite Output Per Hour Vs Time Template reference page for more information. |
| Output By Type (Table) | Creates a table that breaks down the output of an object based on a flow item or token's type or label. It can also track the output of composite objects, so the content between an entry object and an exit object. See the Output By Type Table Template reference page for more information. |
| Output By Type (Bar) | Creates a bar chart that breaks down the output of an object based on a flow item or token's type or label. It can also track the output of composite objects, so the content between an entry object and an exit object. See the Output By Type Bar Template reference page for more information. |
| Output Per Hour By Type (Table) | Creates a table that breaks down the output of an object based on a flow item or token's type or label divided by the total number of hours in the simulation so far. It can also track the output of composite objects, so the content between an entry object and an exit object. See the Output Per Hour By Type Table Template reference page for more information. |
| Output Per Hour By Type (Bar) | Creates a bar chart that displays a breakdown of the output of an object based on a flow item or token's type or label divided by the total number of hours in the simulation so far. It can also track the output of composite objects, so the content between an entry object and an exit object. See the Output Per Hour By Type Bar Template reference page for more information. |
| Output Per Hour By Type (vs Time) | Creates a line graph that show the changes over time in the output of an object based on a flow item or token's type or label for each hour in the simulation so far. It can also track the output of composite objects, so the content between an entry object and an exit object. See the Output Per Hour By Type Vs Time Template reference page for more information. |

 

 

 

## Staytime Templates

 

Staytime templates can be used to graph the staytime values of many individual objects, process flow activities, or a specified set of objects. In other words, these templates can get statistics about how long flow items or tokens remained at an object or activity.

 

The following table provides a brief overview of the available templates:

 

| Name and Icon | Description |
| --- | --- |
| Staytime (Table) | Creates a table that shows the average, minimum, and maximum staytimes for all selected objects. See the Staytime Table Template reference page for more information. |
| Staytime (Bar) | Creates a bar chart that displays the average staytimes for all selected objects. See the Staytime Bar Template reference page for more information. |
| Staytime Per Hour | Creates a line graph that shows the changes over time in the average staytime for all selected objects for each hour that the simulation has run so far. See the Staytime Per Hour Vs Time Template reference page for more information. |
| Staytime (vs Time) | Creates a time plot representing the stay time of each flow item or token that entered and exited the selected objects. See the Staytime vs Time Template reference page for more information. |
| Staytime Histogram | Creates a histogram that represents how many flow items or tokens had a specific staytime value for the selected objects. See the Staytime Histogram Template reference page for more information. |
| Composite Staytime (Table) | Creates a table showing the average, minimum, and maximum combined staytimes for a group of selected objects. The chart will track the staytimes for flow items or tokens between an entry object and an exit object. See the Composite Staytime Table Template reference page for more information. |
| Composite Staytime (Bar) | Creates a bar chart showing the average combined staytimes for a group of selected objects. The chart will track the staytimes for flow items or tokens between an entry object and an exit object. See the Composite Staytime Bar Template reference page for more information. |
| Composite Staytime Per Hour (vs Time) | Creates a line graph that shows the changes over time in the average staytime for a group of selected objects for each hour that the simulation has run so far. The chart will track the staytimes for flow items or tokens between an entry object and an exit object. See the Composite Staytime Per Hour Vs Time Template reference page for more information. |
| Composite Staytime (vs Time) | Creates a time plot that shows each flow item or token's staytime for a group of selected objects. The chart will track the staytimes for flow items or tokens between an entry object and an exit object. See the Composite Staytime vs Time Template reference page for more information. |
| Composite Staytime Histogram | Creates a histogram showing how many flow items or tokens had a specific staytime value for the selected group of objects. The chart will track the staytimes for flow items or tokens between an entry object and an exit object. See the Composite Staytime Histogram Template reference page for more information. |
| Staytime By Type (Table) | Creates a table that breaks down the average, minimum, and maximum staytimes for an object based on a flow item or token's type or label. See the Staytime By Type Table Template reference page for more information. |
| Staytime By Type (Bar) | Creates a bar chart that breaks down the average staytimes for an object based on a flow item or token's type or label. See the Staytime By Type Bar Template reference page for more information. |
| Staytime Per Hour By Type (vs Time) | Creates a line graph that shows the changes over time in the average staytime for an object based on a flow item or token's type or label for each hour that the simulation has run so far. See the Staytime Per Hour By Type Vs Time Template reference page for more information. |
| Staytime By Type (vs Time) | Creates a time plot that breaks down the staytimes for each flow item or token based on its type or label for all selected objects over time. See the Staytime By Type Vs Time Template reference page for more information. |
| Staytime By Type Histogram | Creates a histogram showing how many flow items or tokens had a specific staytime value for the selected group of objects based on the flow item or token's type or label. See the Staytime By Type Histogram Template reference page for more information. |
| Item Trace Gantt | Creates a gantt chart that tracks each flow item's staytime at each object or activity as it flows through the simulation. See the Item Trace Gantt Template reference page for more information. |

 

 

 

## State Templates

 

State templates allow you to make charts that display state information for a set of objects. States give information about how much time a 3D object spends in a particular state such as idle, processing, traveling, etc. Be aware that these templates can only work with fixed resources and task executers in the 3D model.

 

The following table provides a brief overview of the available templates:

 

| Name and Icon | Description |
| --- | --- |
| State Pie | Creates a pie chart that shows the percentage of time that 3D object(s) are in various states. See the State Pie Template reference page for more information. |
| Composite State Pie | Creates a pie chart that shows the average percentage of time that a group of 3D objects is in various states. See the Composite State Pie Template reference page for more information. |
| State Bar | Creates a bar chart that shows the percentage of time that 3D object(s) are in various states. See the State Bar Template reference page for more information. |
| Composite State Bar | Creates a bar chart that shows the total percentage of time that a group of 3D objects is in various states. See the Composite State Bar Template reference page for more information. |
| Utilization By Hour (vs Time) | Creates a time plot that shows the percentage of time in which a 3D object(s) spent utilized during a defined time interval. See the Utilization By Hour Template reference page for more information. |
| State Gantt | Creates a gantt chart that shows the precise amount of time 3D object(s) are in various states over time. See the State Gantt Template reference page for more information. |

 

 

 

## Zone Templates

 

Zone templates allow you to chart statistics from a specific zone shared asset in a process flow. The following table provides a brief overview of the available templates:

 

| Name and Icon | Description |
| --- | --- |
| Zone Statistics (Table) | Creates a table that can show the current, minimum, maximum, and average content of tokens in a process flow zone. See the Zone Statistics Table Template reference page for more information. |
| Zone Partition Statistics (Table) | Creates a table that can show the current, minimum, maximum, and average content of tokens in a process flow divided by their partitions. See the Zone Partition Statistics Table Template reference page for more information. |
| Zone Statistics (Bar) | Creates a bar chart that can display any of the standard zone statistics available for a zone in process flow. See the Zone Statistics Bar Template reference page for more information. |
| Zone Partition Statistics (Bar) | Creates a bar chart that can display any of the standard zone statistics available for a zone in process flow divided by token partitions. See the Zone Partition Statistics Bar Template reference page for more information. |
| Zone Stats (vs Time) | Creates a time plot that shows the value of any standard zone statistic as it changes over time. See the Zone Statistics Vs Time Template reference page for more information. |
| Zone Partition Stats (vs Time) | Creates a time plot that shows the value of any standard zone statistic as it changes over time divided by token partitions. See the Zone Partition Statistics Vs Time Template reference page for more information. |

 

 

 

## Milestone Templates

 

Milestone templates allow you to chart statistics gathered by a milestone collector. The following table provides a brief overview of the available templates:

 

| Name and Icon | Description |
| --- | --- |
| Interval Histogram | Creates a histogram showing how many flow items or tokens had a specific staytime value for the selected milestone intervals. See the Interval Histogram Template reference page for more information. |
| Interval By Type Histogram | Creates a histogram showing how many flow items or tokens had a specific staytime value for the selected milestone intervals based on the flow item or token's type or label. See the Interval By Type Histogram Template reference page for more information. |
| Interval | Creates a box plot that shows the distribution and quartiles of the selected milestone intervals. See the Interval Box Plot Template reference page for more information. |
| Interval By Type | Creates a box plot that shows the distribution and quartiles of the selected milestone intervals based on the flow item or token's type or label. See the Interval By Type Box Plot Template reference page for more information. |
| Milestone Sankey | Creates a sankey diagram that shows how flow items or tokens flow from one milestone to the next. See the Milestone Sankey Template reference page for more information. |
