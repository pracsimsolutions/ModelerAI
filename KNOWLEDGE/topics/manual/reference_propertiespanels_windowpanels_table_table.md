---
id: reference_propertiespanels_windowpanels_table_table
name: "Table"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","WindowPanels","Table","Table"]
tags: ["manual","reference","propertiespanels","windowpanels"]
source: "manual/Reference/PropertiesPanels/WindowPanels/Table/Table.html"
---

# Table

The Table panel provides a graphical interface for editing Global Tables and other Tables and doing many of the things that can be done programatically through the Table FlexScript API.

 

 

The following properties are on the Table panel:

 

### Name Combobox

 

This is the table's name and has a list of all of the model's Global Tables. The name should be memorable and describe the table's function. The commands to read and write to global tables access them by name. You can view other Global Tables in this window by clicking the dropdown arrow next to the name.

 

### Rows

 

Change the number of rows in the table.

 

### Columns

 

Change the number of columns in the table.

 

### Use Bundle

 

If this box is checked, the internal data type of the table will be changed to bundles. See Bundle Data.

 

### Buttons

 

- - This button adds the table as a row in the Excel Importer or Excel Exporter.
- - Pins the entire global table to a dashboard as either a table of values, bar chart or line graph.
- - This button lets you add this table to a user library as either a draggable icon or as a component for automatic install.

 

### On Reset Trigger

 

The On Reset trigger gives you the ability to define what happens to the table's data when the model is reset. This could be clearing all cell data, delete all rows, or some other custom defined behavior.

 

### Go To

 

Scrolls the table view to show the specified row and column.

 

### Description

 

This text box allows you to document your model by writing a description of the table. Here you can explain the purpose and organization of your table.

  

#### Other Tables

 

When editing a table or array that is not a Global Table, some options in the panel will not be available.
