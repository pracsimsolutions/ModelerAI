---
id: reference_tools_globaltables_globaltables
name: "Global Tables"
kind: manual
breadcrumb: ["Reference","Tools","GlobalTables","GlobalTables"]
tags: ["manual","reference","tools","globaltables"]
source: "manual/Reference/Tools/GlobalTables/GlobalTables.html"
---

# Global Tables

## Overview and Key Concepts

 

Global Tables can store numbers, strings, pointers, arrays, FlexScript, bundles or tracked variables. This data can be accessed by any object in the model using the Table class interface. A model may have any number of Global Tables.

 

 

Global Tables are accessed from the Toolbox.

 

 

 

## Getting and Setting Table Data

 

Here are some code examples of getting and setting table data.

 

```
		Table myTable = Table("GlobalTable1");
		myTable[1][1] = 5;
		myTable[2]["MyCol"] = "Test";
		item.LabelName = myTable[1][2];
		double value = Table("GlobalTable1")[1]["Col 3"];
```

 

For more information, see the Table FlexScript API.

 

 

 

## Editing the Table

 

To edit a cell in the table, click the desired cell and begin typing to overwrite all data in the cell, or double click on the cell to select the cell's contents. You can also right-click a cell and select Explore As Table or Explore As Tree and edit the cell's data through there. If the cell has array or bundle data, double-clicking the cell will open a new table window for editing.

 

Use the arrow keys to navigate between cells. Cells hold number data by default. You can change a cell's data type by right-clicking on the cell and selecting an option under the Assign Data menu. The right-click menu also has options for insert/deleting rows and columns, clearing cell data, and sorting by column.

 

Other options, like resizing the table, can be found in Properties.

  

#### Similar Windows

 

A similar window to this is used when editing a label table from an object's labels or when editing a node or array as a table (accessible through the right-click menu in the Tree Window or Labels page). When editing a table or array that is not a Global Table, some options in Properties will not be available.

  

 

 

## Bundle Data

 

If the "Use Bundles" checkbox is checked, your table will change internally to use the bundle data type. This means that your table will take significantly less memory. However, there are some limitations. All data in a column must be of one type (number, string, or node) and rows can no longer be named. The data type of a column can be changed by right-clicking in a cell in the column. All of the normal table functions will work with the bundle data type.

 

Using bundles for your table data is highly recommended for large tables with over 500 rows or columns.

 

 

 

## Cell Data Types

 

Cells in a table can store a variety of different types of data. These include:

 

- **Numbers:** Integer or floating point.
- **Strings:** Text like names or sentences.
- **Pointers:** References to other nodes or objects. The value displayed is the path to the node/object and will be purple.
- **Arrays:** Arrays can have any number of elements. Each element can store a number, string, pointer or array.
- **FlexScript:** FlexScript toggled nodes allow you to write code that can that be evaluated using the evaluate() method. For example, `Table("GlobalTable1").cell(1, 1).evaluate()`
- **Bundles:** Data table that stores numbers, strings, and nodes.
- **Tracked Variables:** Tracked variables are often used for statistic collection.

 

Additionally, a cell can also store a table of data. This can be done regardless of the data stored on the cell. To create a table on a cell, right click on the desired cell and select **Explore As Table**. This will open a new table window where you can add rows and columns to the cell. If a cell has a table, a small gray triangle will be drawn in the upper right corner of the cell.

 

Tracked Variable cells will display with a small green triangle in the upper right corner.

 

If a cell has array or bundle data, double-clicking on the cell will open a table view that can be edited.

 

 

 

## Properties Panels

 

The Global Table uses the following properties panels:

 

- Table
