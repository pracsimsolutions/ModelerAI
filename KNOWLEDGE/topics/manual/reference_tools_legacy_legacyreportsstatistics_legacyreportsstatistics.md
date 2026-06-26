---
id: reference_tools_legacy_legacyreportsstatistics_legacyreportsstatistics
name: "Legacy Reports and Statistics Tool"
kind: manual
breadcrumb: ["Reference","Tools","Legacy","LegacyReportsStatistics","LegacyReportsStatistics"]
tags: ["manual","reference","tools","legacy"]
source: "manual/Reference/Tools/Legacy/LegacyReportsStatistics/LegacyReportsStatistics.html"
---

# Legacy Reports and Statistics Tool

## Overview and Key Concepts

 

The Reports and Statistics dialog box allows the modeler to create various reports based on statistics gathered during a model run. These reports can include information about flow item throughput, staytime, state history and other data that the modeller can select or customize. This dialog box also allows the modeler to create a document containing the most important details about objects in the model.

 

 

 

## The Summary Report Tab

 

The standard report tab allows you to create a report of your model. FlexSim reports on a list of attributes that you define for the model. Once you have created the list of attributes that you want to report, click on the Generate Report button. The report is then created and exported to Microsoft Excel.

 

It has the following properties:

 

 

### Standard Attributes

 

This list lets you select standard attributes like content, staytime, state variable, etc. Press the button at the top to add the selected attribute to the list of attributes to report.

 

### User Defined Attributes

 

Here you can type in the name of a label or variable that you want reported, then click the button at the bottom to add your own attributes to the report list. For example, if one or more Queues have a label called "lastreditem" and you want a report of all such labels and their values, then type "lastreditem" in the field and press the button.

 

### Attributes to Report

 

This is the list of attributes that will be reported. To remove an item from the list, select it and press the button.

 

### Generate Report

 

Press this button to generate the report.

 

 

 

## The State Report Tab

 

The state report tab allows you to create a report of the time the objects in your model spent in individual states. This can be reported either as an exact time or as a percentage of the model run time. Once you have created the list of states that you want to report, click on the Generate Report button. The report is then created and exported to Microsoft Excel.

 

It has the following properties:

 

 

### Display values as percentages

 

If this box is checked the report will display the percentage of the total run time that the objects spent in each state. If it is not checked, the report will display the exact amount of time the objects spent in each state.

 

### Available States

 

These are the states available in FlexSim that can be included in the report. To place a state in the States to Report column, highlight the state you are interested in and press the button. You can place all of the available states in the States to Report column by pressing the button.

 

### States to Report

 

The time the objects spent in these states will be displayed in the report. The time for all of these states will be reported for all objects, even if the object was never in some of the states. To remove a state from this list, highlight it and press the button. To remove all of the states from this list press the button.

 

### Generate Report

 

Press this button to generate the report.

 

 

 

## The Model Documentation Tab

 

The model documentation tab lets you create a (.txt) document that reports information on your model. Check the appropriate boxes that you want to be documented, and then click the Generate Report button to create the file.

 

It has the following properties:

 

 

### Objects

 

These options allow the modeler to select which attributes of the model objects should be included in the report. If "Group by Object" is selected, all of the selected attributes for each object will be together in the final report. If "Group by Category" is selected, then all of the values for each attribute will be together in the report. If "Verbose" is selected, then any code fields (triggers, process time, etc.) will be documented with the full text of the field. If "Verbose" is not checked, then only the text that appears a template window will be recorded in the resulting document.

 

### Global

 

These options allow the modeler to select which global objects and features they would like included in the report.

 

### Generate Report

 

Press this button to generate the report.

 

 

 

## The Options Tab

 

The options tab allows you to select the classes of objects that will be displayed in the reports.

 

It has the following properties:

 

 

### Available Classes

 

These are all of the classes in FlexSim whose instances can be included in reports. These are the classes that appear the Library Icon Grid. You can add a class to the Classes to Report list by highlighting the class you are interested in and pressing the button. You can add all of the classes to the Classes to Report list by pressing the button.

 

### Classes to Report

 

All of the objects in the model that are instances of any of these classes will be included in any reports that are generated. Any instances of classes that are not in this list will not be included in reports. To remove a class from this list, highlight the class you want to remove and press the button. You can remove all of the classes from the list by pressing the button.
