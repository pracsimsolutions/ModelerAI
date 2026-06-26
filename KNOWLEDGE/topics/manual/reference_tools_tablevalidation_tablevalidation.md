---
id: reference_tools_tablevalidation_tablevalidation
name: "The Table Validation Tool"
kind: manual
breadcrumb: ["Reference","Tools","TableValidation","TableValidation"]
tags: ["manual","reference","tools","tablevalidation"]
source: "manual/Reference/Tools/TableValidation/TableValidation.html"
---

# The Table Validation Tool

## Overview and Key Concepts

 

The Table Validation tool allows you to enumerate your assumptions about the data in specific tables and to test them, ensuring those tables contain valid data. For general information on how to use the Table Validation tool, see the Validating Input Data topic.

 

### The Validation Algorithm

 

There may be cases where you need to write your own constraint functions or On Fail logic. Understanding how the Table Validation tool uses those components can be helpful. When the Table Validation tool validates data, it uses the following algorithm:

 

The tool loops over all ranges in all tables in table order and then in range order, as they are seen in the user interface. In this way, it visits each range.

 

When visiting a range, the tool creates a **context** associated with each rule connected to that range. The context stores a copy of the default **Error Message** for the rule. Then, the tool scans each row in the range from top to bottom. Within the row, cells are scanned from left to right.

 

When scanning a cell, the tool calls the **Constraint** function of each connected rule. Rules are tested in the order seen on the Rules tab. For each rule, the tool passes the cell's value and the context associated with the rule into the constraint function. If the function returns anything other than zero, then the value passes the rule.

 

If a Constraint function returns zero, then the tool calls the rule's **On Fail** callback. This callback allows the user to try to repair the value or update the error message. If the On Fail callback returns 0, the error message is logged.

 

The following pseudocode presents a summary of the algorithm used:

 

```
for each table:
    for each range:
        for each rule:
            create a context for the rule
        
        for each row in the range:
            for each column in the row:
                value := table[row][col]
                for each rule:
                    context := context associated with this rule    
                    result := constraint(value, context)
                    if result = 0:
                        result := onFail(value, context)
                        if result = 0:
                            log an error

        for each rule:
            delete the context
```

 

A critical part of this algorithm is the context. The context is a Simple Data Type, similar to a Process Flow token. The context has several purposes:

 

- The context can store labels. This allows you to store data between invocations of the constraint function. For example, a rule that checks whether a column is sorted would use the context to store the previous value. A rule that checks the sum of a column would use the context to store the current total.
- The context provides read-only access to many variables about the algorithm, for example the current table, row and column.
- The context provides access to the rule's error message, so you can optionally customize the message that is logged on a per-value basis, rather than per rule.
- The context provides access to the current value of the table so you can optionally repair invalid values in the On Fail callback.

 

 

 

## The Table Ranges Tab

 

The Table Ranges tab has the following properties:

 

 

### Table List

 

The table list allows you to edit the Table Validation tool's list of table references. Select an item in the list to display its properties to the right of the list.

 

### Table Area

 

The table area is in the upper right region of the Table Ranges tab. The table area has the following properties:

 

- **Name** - Edit the name of the table reference. This name appears in error messages if the table is invalid.
- **Table** - Edit the reference to a table. This node is evaluated to get a reference to a table. It is usually a pointer, but can also contain the name of a Global Table, or code that returns a Table.

 

### Ranges Area

 

The ranges area is in the lower right region of the Table Ranges tab. The ranges area has the following properties:

 

- **Range List** - The range list allows you to edit the set of ranges within the selected table.
- **Name** - Edit the name of the currently selected range. The name of the range appears in logged error messages.
- **Range Type** - Edit the type of range. The possible values are as follows:
- **Column** - A range that includes all values in the column specified by the **Range Value**. If the target table has no rows, this range can be empty.
- **Row** - A range that includes all values in the row specified by the **Range Value**.
- **Column headers** - A range that includes all of the column headers.
- **Row headers** - A range that includes all of the row headers. If the target table has no rows, this range can be empty.
- **All values** - A range that includes all cells in the body of the table, excluding row and column headers.
- **Custom** - A custom range is specified by the user. In this case, the **Range Value** must return an array of four values: the start row, start column, end row, and end column. If there are no rows, this range will be empty.

  **Range Value** - Edit the range value, if the current **Range Type** requires a range value. **Rules** - Edit the set of rules that should be checked by this range as part of validation.  

 

 

## The Rules Tab

 

The Rules tab has the following properties:

 

 

### The Rules List

 

You can use the Rules list to edit the set of available rules in this Table Validation tool. During validation, rules are checked in the order specified by this list.

 

### Name

 

The Name property allows you to edit the name of the rule. The name of the rule appears in logged error messages.

 

### Constraint

 

The Constraint property defines the function used to test values. You can edit the code of this field to define a function. That function should return a one if the value passes the test or a zero if it fails.

 

### Fail if Range Empty

 

Check this box to log an error if a range connected to this rule is empty. This box should only be unchecked if attached to a range type that can be empty (Column, Row Headers, and Custom) and if the source table is valid with zero rows (such as a query with no matching rows).

 

### Error Message

 

The Error Message property edits the default error message of the rule. This message is shown in the error log if a value fails the rule.

 

### On Fail

 

The On Fail property defines a callback function. This function is optional. You can use it to repair an invalid value and/or update the error message. This function returns a one or a zero. If it returns a zero, the error is logged. If it returns a 1, the value is considered acceptable and no error is logged.

 

 

 

## The Logged Errors Tab

 

The Logged Errors tab has the following properties:

 

 

### Error Table

 

When you click the Validate button, the Error Table displays a list of all errors encountered during validation. If a given error has a valid table, row, and column, you can double-click on that error to open the table view and select the invalid cell.
