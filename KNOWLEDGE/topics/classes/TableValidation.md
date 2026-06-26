---
id: tablevalidation
name: "TableValidation"
kind: class
module: ""
signature: "TableValidation"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a Table Validation."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TableValidation\\TableValidation.xml"
---

# TableValidation

A class that represents a Table Validation.

## Methods

### TableValidation

```flexscript
TableValidation TableValidation.TableValidation(string name)
```

Creates a reference to a Table Validation tool.

**Parameters:**
- `name` *(string)* — The name of a Table Validation tool in the Toolbox.

TableValidation validator1 = TableValidation("TableValidation1");

### validate

```flexscript
int TableValidation.validate(treenode errorLoggingNode = 0)
```

Validates all tables, their rules and ranges defined in the TableValidation tool and returns the number of errors.

**Parameters:**
- `errorLoggingNode` *(treenode, default 0)* — The treenode where the errors will be logged. If no treenode is passed in, errors will be printed to the system console. If the node has text data, the errors will instead be printed to the node's data. For other nodes (including bundle nodes) the node is treated as a Table. Each error is stored as a row in the table.

Validates the data specified in the various ranges against the rules. The first 100 rule violations
are logged as an error. The method returns the total number of errors encountered, even if they weren't
logged.

Example:

return TableValidation("TableValidation1").validate();

