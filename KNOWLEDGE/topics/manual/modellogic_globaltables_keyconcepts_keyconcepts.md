---
id: modellogic_globaltables_keyconcepts_keyconcepts
name: "Key Concepts About Global Tables"
kind: manual
breadcrumb: ["ModelLogic","GlobalTables","KeyConcepts","KeyConcepts"]
tags: ["manual","modellogic","globaltables","keyconcepts"]
source: "manual/ModelLogic/GlobalTables/KeyConcepts/KeyConcepts.html"
---

# Key Concepts About Global Tables

## Global Tables Overview

 

A Global Table is the primary way to work with table data in FlexSim:

 

 

Global Tables are called "Global" because they can be accessed by name from anywhere in a model. You can access them in 3D object properties or triggers or in Process Flow code fields. In many cases, you can find a pickoption to look up values in a Global Table.

 

Global Tables are commonly used to make table-driven models. A table-driven model is a model that uses table data to determine its behavior. For example, in a warehousing model, tables could specify the number and location of pallets for initial inventory. In a healthcare model, a table could specify the types of patients each staff member can treat. In a manufacturing model, a table could specify setup times for various machines, based on the machine type, the previous SKU, and the next SKU.

 

While models are not required to use Global Tables, most models will use several or even dozens. Global Tables are a good organizational tool, allowing you to view and edit dataq in a convenient way. In addition, Global Tables can be used to make a model more flexible. If a model's behavior is determined by tables, then changing table data will change the behavior of the model.

 

Often, the end user of a model may be more familiar with Microsoft Excel than with FlexSim. In these cases, the model can be built so that to adjust certain aspects of model behavior, they just have to edit an Excel workbook. Then FlexSim will pull the data from the various sheets of the workbook into the its Global Tables, and run accordingly.

 

 

 

## Structure of a Global Table

 

A Global Table is a rectangular grid of cells, where each cell contains a value. A cells's vertical location is called its **row** and a cell's horizontal location is called its **column**. In addition, each row has a name called a **row header**, and each column has a name called a **column header**. These elements are shown in the following figure:

 

 

It is common to designate a value's location using two pairs of square brackets. For example, if the above table were called MyTable, you could access the value in the fourth row and third column with code like this:

 

```
Table("MyTable")[4][3] // yields 5.60
```

 

Also, when you click on a single cell in a Global Table, the main status bar will use square brackets to indicate what is selected. In the following example, the cell on row 3, column 2 is selected:

 

 

In addition to accessing values by their row and column number, you can also access values by their row and column header:

 

```
Table("MyTable")["Row 4"]["Col 3"] // yields 5.60
```

  

#### Row and Column Headers are Optional

 

A Global Table doesn't need to have row headers or column headers. They are simply user-defined names for the row or column. For clarity and ease of use, we recommend specifying column headers.

   

#### You can use Global Tables Without Writing Code

 

Because Global Tables are so common, FlexSim provides many interfaces that allow you to use a table without writing custom FlexScript code.

  

 

 

## Adding Global Tables to a Model

 

Global Tables are added to a model using the **Toolbox** window. If the Toolbox window isn't visible, you can open it by selecting **View** in the main menu, and then choosing **Toolbox**.

 

Once the Toolbox is open, you can click the green plus button and choose **Global Table** from the button's menu.

 

 

When you first create a Global Table, a Global Table view will appear showing the contents of the table. In addition, the Properties panel will update allowing you to view and edit the properties of the table.

 

You can close the view at any time. To re-open the view, you can double-click on the table's icon in the Toolbox.

 

 

 

## Editing Global Table Data

 

This section describes using FlexSim's interface to edit Global Tables. For information on importing external data (e.g. from Excel) into a Global Table, see the next topic: Importing Data into Global Tables.

 

### Adding Rows and Columns

 

With the Global Table view open, the Properties window will display a Global Table panel. On this panel you can adjust the number of rows or columns in the table.

 

 

For more information about the Global Table panel, see the Table Panel topic.

 

### Editing Values

 

Each cell in the table has a **data type** and a **value** of that type. To change the value of the cell, click the desired cell and begin typing to overwrite all data in the cell, or double click on the cell to select the cell's contents.

 

To change the data type of a cell, right-click on the cell and point to **Assign Data**, and choose the desired data type. The two most common data types are **number data** and **string data**. Assigning number data enables the cell to store numbers. Assigning string data enables the cell to store text. For more information cell values and data types, see the Global Table Reference topic.

  

#### Assign Data Types to a Row or Column

 

You can assign the data type of an entire column or row by right-clicking on the column header or row header. When you pick an option, it will be applied to the entire column or row.

  

While manual data entry is possible, most models use data from external sources. To learn about how to import data from outside FlexSim, continue to the next topic: Importing Data into Global Tables.
