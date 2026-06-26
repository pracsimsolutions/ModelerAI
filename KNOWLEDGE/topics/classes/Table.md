---
id: table
name: "Table"
kind: class
module: ""
signature: "Table"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a table structure."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Table.xml"
---

# Table

A class that represents a table structure.

A Table contains methods and properties associated with manipulating table
data in FlexSim. A Table may be one of the following:

A Tree Table: This is a table whose data is defined in the tree structure. This
consists of a root node with one or more subnodes, each representing a row in the table. Subnodes
of each row represent cells in the table.

A Bundle Table: This is a table whose data is defined by a bundle node. This
consists of a node in the tree that contains bundle data.

A Query Result Table: This is a read-only in-memory table that contains the result
of a call to Table.query().

An Array Table: This is an in-memory table that references a two-dimensional array.

Bundle and tree tables can be stored anywhere in the model tree. Often they are stored as global tables,
or on object labels.

## Methods

### query

```flexscript
Table (static) Table.query(string queryStr, lambda p1 ... p9 = 0)
```

Executes an SQL query, returning the result as a Table.

**Parameters:**
- `queryStr` *(string)* — The sql query.
- `p1 ... p9` *(lambda, default 0)*

**Returns:** The resulting table. This is a read-only in-memory table (it is not stored on a node in the tree). Use Table.cloneTo() to copy the result to a tree-bound table.

Table result = Table.query("SELECT * FROM Customers ORDER BY Name");

See Sql Query Examples for more information.

### addCol

```flexscript
Table Table.addCol(int col = 0, int datatype = 0)
```

Adds a column to the table.

**Parameters:**
- `col` *(int, default 0)* — The index at which to insert the column.
- `datatype` *(int, default 0)* — The datatype of the new column.

**Returns:** The table.

table.addCol(2);
If no column index is specified the column will be added at the end.
If no datatype is specified the new column will copy the datatype of the previous column.

### addIndex

```flexscript
Table.addIndex(Variant keyCol, int indexType = Table.IndexType.Unordered)
```

Adds an index to the specified column.

**Parameters:**
- `keyCol` *(Variant)* — A column name or number
- `indexType` *(int, default Table.IndexType.Unordered)* — The type of index desired. Can be either Table.IndexType.Unordered or Table.IndexType.Ordered. Generally the default unordered value is sufficient. Table.IndexType.Ordered can be useful if you are querying the table with Table.query() with a WHERE filter that uses a >, <, >=, or <= comparison on the column.

An index can significantly improve the performance of
the following operations:

Using the Table.getRowByKey() method with the indexed column
Using the Table.getValueByKey() method with the indexed column
Finding a value from the indexed column in a query (e.g. WHERE IndexedCol = SomeValue

An index can significantly slow performance to operations that change the table. If
a table changes frequently during the model run, an index is not recommended.
You can only add an index to a column in a Bundle table.

### addRow

```flexscript
Table Table.addRow(int row = 0, int datatype = 0)
```

Adds a row to the table.

**Parameters:**
- `row` *(int, default 0)* — The index at which to insert the row.
- `datatype` *(int, default 0)* — The datatype of the new row.

**Returns:** The table.

table.addRow(2);

If no row index is specified the row will be added at the end.
If no datatype is specified the cells will copy the datatype of the cells in the previous row.

### cell

```flexscript
treenode Table.cell(Variant row, Variant col)
```

Returns a specific table cell.

**Parameters:**
- `row` *(Variant)* — The row index.
- `col` *(Variant)* — The column index.

**Returns:** The cell's node in the tree.

table.cell(2,"Row 3").value = 4;
This will throw an exception for bundles.

### clear

```flexscript
Table Table.clear(int recursive = 0)
```

Sets all cells to empty values.

**Parameters:**
- `recursive` *(int, default 0)* — If set to 1, clear will be called on cells with bundle data, array data and on cells that have node tables.

**Returns:** The table.

table.clear();
table.clear(1);
Number cells will be set to 0, string cells to an empty string, and pointer cells will be set to a null pointer.

### clone

```flexscript
Array Table.clone()
```

Creates an array copy of the table.

**Returns:** A copy of the table in an array.

Array myArray = Table("Parts").clone();

### cloneTo

```flexscript
Table Table.cloneTo(Table target)
```

Copies all values from this table to the target table, returning the target table.

**Parameters:**
- `target` *(Table)* — The target table. This must be a tree-bound table, such as a bundle or tree table.

**Returns:** The resulting table. This is the same table as the target.

Table.query("SELECT * FROM Customers ORDER BY Name").cloneTo(Table("QueryDump"));

### deleteCol

```flexscript
Table Table.deleteCol(int col)
```

Deletes a table column.

**Parameters:**
- `col` *(int)* — The index of the column to delete.

**Returns:** The table.

table.deleteCol(3);
Will throw an exception if no column is specified or the column doesn't exist.

### deleteRow

```flexscript
Table Table.deleteRow(int row)
```

Deletes a table row.

**Parameters:**
- `row` *(int)* — The index of the row to delete.

**Returns:** The table.

table.deleteRow(3);
Will throw an exception if no row is specified or the row doesn't exist.

### executeCell

```flexscript
Variant Table.executeCell(Variant row, Variant col)
```

Executes the text on the cell as FlexScript.

**Parameters:**
- `row` *(Variant)* — The row index.
- `col` *(Variant)* — The column index.

**Returns:** The return value of the executed function.

Variant result = table.executeCell(2, "Row 3");

### getColHeader

```flexscript
string Table.getColHeader(int colNum)
```

Gets a table column header.

**Parameters:**
- `colNum` *(int)* — The column index.

**Returns:** The text in the column header.

string header = table.getColHeader(2);

### getColNum

```flexscript
int Table.getColNum(string colHeader)
```

Gets the index of a table column header.

**Parameters:**
- `colHeader` *(string)* — The column header.

**Returns:** The index of the column header.

int col = table.getColNum("Orders");
Returns 0 if the column header doesn't exist.

### getRowByKey

```flexscript
int Table.getRowByKey(Variant key, Variant keyCol = nullvar)
```

Searches a column for the specified key value.

**Parameters:**
- `key` *(Variant)* — The value to use as the key
- `keyCol` *(Variant, default nullvar)* — The column to search for the key value.

**Returns:** The row number containing the key value in the specified column. If no match was found, returns 0.

If the keyCol is not specified, the first indexed
column in the table will be searched. If there is no indexed column, the first
column (column 1) will be searched.

If the column to search has an index, then the index will be used to look up
the correct row, rather than searching row by row.

### getRowHeader

```flexscript
string Table.getRowHeader(int rowNum)
```

Gets a table row header.

**Parameters:**
- `rowNum` *(int)* — The row index.

**Returns:** The text in the row header.

string header = table.getRowHeader(2);

### getRowNum

```flexscript
int Table.getRowNum(string rowHeader)
```

Gets the index of a table row header.

**Parameters:**
- `rowHeader` *(string)* — The row header.

**Returns:** The index of the row header.

int row = table.getRowNum("Type");
Returns 0 if the row header doesn't exist.

### getValueByKey

```flexscript
Variant Table.getValueByKey(Variant key, Variant valueCol, Variant keyCol = nullvar)
```

Searches a column for the specified key value, and returns one or more values from the matching row.

**Parameters:**
- `key` *(Variant)* — The value to use as the key
- `valueCol` *(Variant)* — A column (name or number) to retrieve data from.
- `keyCol` *(Variant, default nullvar)* — The column to search for the key value.

**Returns:** The value in the valueCol in the matching row.

If the keyCol is not specified, the first indexed
column in the table will be searched. If there is no indexed column, the first
column (column 1) will be searched.

If the column to search has an index, then the index will be used to look up
the correct row, rather than searching row by row.

Once the matching row is found, the value in valueCol on that
row is retrieved. If valueCol is an array, the values from
each specified column will be retrieved, and returned as an array.

### hasIndex

```flexscript
int Table.hasIndex(Variant keyCol)
```

Returns the index type of the specified column if it has an index, otherwise 0.

**Parameters:**
- `keyCol` *(Variant)* — A column name or number

**Returns:** If the specified column has an index, returns either Table.IndexType.Ordered or Table.IndexType.Unordered.

An index can be added using the
Table.addIndex()
method.

### moveCol

```flexscript
Table Table.moveCol(int col, int newCol)
```

Moves a column to a new index.

**Parameters:**
- `col` *(int)* — The index of the column to move.
- `newCol` *(int)* — The index where the column will be moved.

**Returns:** The table.

table.moveCol(2, 5);

### moveRow

```flexscript
Table Table.moveRow(int row, int newRow)
```

Moves a row to a new index.

**Parameters:**
- `row` *(int)* — The index of the row to move.
- `newRow` *(int)* — The index where the row will be moved.

**Returns:** The table.

table.moveRow(2, 5);

### removeIndex

```flexscript
Table.removeIndex(Variant keyCol)
```

Removes an index from the specified column

**Parameters:**
- `keyCol` *(Variant)* — A column name or number.

An index can be added using the
Table.addIndex()
method. If the specified column does not have an index,
this method does nothing.

### setColHeader

```flexscript
Table Table.setColHeader(int colNum, string name)
```

Renames a column header.

**Parameters:**
- `colNum` *(int)* — The column index.
- `name` *(string)* — The new column header name.

**Returns:** The table.

table.setColHeader(2, "Orders");

### setRowHeader

```flexscript
Table Table.setRowHeader(int rowNum, string name)
```

Renames a row header.

**Parameters:**
- `rowNum` *(int)* — The row index.
- `name` *(string)* — The new row header name.

**Returns:** The table.

table.setRowHeader(2, "Order 2");

### setSize

```flexscript
Table Table.setSize(int rows, int cols, int datatype = 0, int overwrite = 0)
```

Resizes the table.

**Parameters:**
- `rows` *(int)* — The new number of rows.
- `cols` *(int)* — The new number of columns.
- `datatype` *(int, default 0)* — The datatype of new cells.
- `overwrite` *(int, default 0)* — If old cells should have their values reset.

**Returns:** The table.

table.setSize(5, 3);

### sort

```flexscript
Table Table.sort(Variant columns, Variant sortDirections = 0)
```

Sorts table columns.

**Parameters:**
- `columns` *(Variant)* — The column or columns to sort.
- `sortDirections` *(Variant, default 0)* — The directions each column should be sorted. 0 is Descending and 1 is Ascending.

**Returns:** The table.

table.sort(1);
table.sort([1, 2, 3], [1, 0, 1]);

### swapCols

```flexscript
Table Table.swapCols(int col, int col2)
```

Swaps indexes of the two columns.

**Parameters:**
- `col` *(int)* — The index of the first column.
- `col2` *(int)* — The index of the ssecond column.

**Returns:** The table.

table.swapCols(2, 4);

### swapRows

```flexscript
Table Table.swapRows(int row, int row2)
```

Swaps indexes of the two rows.

**Parameters:**
- `row` *(int)* — The index of the first row.
- `row2` *(int)* — The index of the second row.

**Returns:** The table.

table.swapRows(2, 4);

### Table

```flexscript
Table Table.Table(string name)
```

Creates a reference to a table.

**Parameters:**
- `name` *(string)* — The name of a global table.

If you pass in a string, FlexSim will
search for a globally-defined table in the following order:

Global Tables
Statistics Collectors
Calculated Tables
State Tables
Property Tables

Table parts = Table("Parts");
int partCount = Table("Parts")[3][4];

## Properties

### numCols

```flexscript
int Table.numCols (readonly)
```

The number of columns in the table.

int columns = table.numCols;
For bundles it is the number of fields.

### numRows

```flexscript
int Table.numRows (readonly)
```

The number of rows in the table.

int rows = table.numRows;
For bundles it is the number of entries.

