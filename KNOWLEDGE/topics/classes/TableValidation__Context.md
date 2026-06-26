---
id: tablevalidation__context
name: "TableValidation::Context"
kind: class
module: "TableValidation"
signature: "TableValidation::Context"
aliases: []
tags: ["class", "tablevalidation-module"]
deprecated: false
replacedBy: null
description: "A class that represents a TableValidation Context. The context stores data that persists through the duration of a rule."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TableValidation\\TableValidation.Context.xml"
---

# TableValidation::Context

A class that represents a TableValidation Context. The context stores data that persists through the duration of a rule.

## Methods

### addRegex

```flexscript
void TableValidation::Context.addRegex(RegExp pattern, string name)
```

Stores a regular expression by name in the context.

**Parameters:**
- `pattern` *(RegExp)* — The pattern of characters to look for, a regular expression.
- `name` *(string)* — The name of the pattern.

TableValidation.Context ctx = param(2);
ctx.addRegex(/^[a-zA-Z]$/, "matchRegex");

### valueMatchesRegex

```flexscript
int TableValidation::Context.valueMatchesRegex(Variant value, string regexName)
```

Compares the value against a stored regular expression in the context.

**Parameters:**
- `value` *(Variant)* — The value you are matching the regular expression to.
- `regexName` *(string)* — the name of the regex stored on the context using addRegex.

**Returns:** If it matched.

Variant value = param(1);
TableValidation.Context ctx = param(2);
ctx.valueMatchesRegex(value, "matchRegex");

## Properties

### col

```flexscript
int TableValidation::Context.col (readonly)
```

Gets the column the value is in.

TableValidation.Context ctx = param(2);
int column = ctx.col;

### endCol

```flexscript
int TableValidation::Context.endCol (readonly)
```

Gets the last column of the range.

TableValidation.Context ctx = param(2);
int endColumn = ctx.endCol;

### endRow

```flexscript
int TableValidation::Context.endRow (readonly)
```

Gets the last row of the range.

TableValidation.Context ctx = param(2);
int endRow = ctx.endRow;

### errorMessage

```flexscript
string TableValidation::Context.errorMessage
```

Gets/sets the errorMessage.

The errorMessage is by default the Failure Message defined on the rule.
It is what is displayed at the end of a logged error message.

TableValidation.Context ctx = param(2);
ctx.errorMessage = "The value was not an integer";

### isFirstCol

```flexscript
int TableValidation::Context.isFirstCol (readonly)
```

returns a 1 if the column the value is in is the first column of the range.

TableValidation.Context ctx = param(2);
int isFirstCol = ctx.isFirstCol;

### isFirstRow

```flexscript
int TableValidation::Context.isFirstRow (readonly)
```

returns a 1 if the row the value is in is the first row of the range.

TableValidation.Context ctx = param(2);
int isFirstRow = ctx.isFirstRow;

### isFirstValue

```flexscript
int TableValidation::Context.isFirstValue (readonly)
```

returns a 1 if the value is the first value in the range.

TableValidation.Context ctx = param(2);
int isFirstValue = ctx.isFirstValue;

### isLastCol

```flexscript
int TableValidation::Context.isLastCol (readonly)
```

returns a 1 if the column the value is in is the last column of the range.

TableValidation.Context ctx = param(2);
int isLastCol = ctx.isLastCol;

### isLastRow

```flexscript
int TableValidation::Context.isLastRow (readonly)
```

returns a 1 if the row the value is in is the last row of the range.

TableValidation.Context ctx = param(2);
int isLastRow = ctx.isLastRow;

### isLastValue

```flexscript
int TableValidation::Context.isLastValue (readonly)
```

returns a 1 if the value is the last value in the range.

TableValidation.Context ctx = param(2);
int isLastValue = ctx.isLastValue;

### labels

```flexscript
TableValidation::Context.labels (readonly)
```

Accesses the context's labels as nodes.

Works just like labels on treenodes.

### row

```flexscript
int TableValidation::Context.row (readonly)
```

Gets the row the value is in.

TableValidation.Context ctx = param(2);
int column = ctx.col;

### startCol

```flexscript
int TableValidation::Context.startCol (readonly)
```

Gets the starting column of the range.

TableValidation.Context ctx = param(2);
int column = ctx.col;

### startRow

```flexscript
int TableValidation::Context.startRow (readonly)
```

Gets the starting row of the range.

TableValidation.Context ctx = param(2);
int column = ctx.col;

### table

```flexscript
Table TableValidation::Context.table (readonly)
```

Gets table that is currently being validated.

TableValidation.Context ctx = param(2);
Table table = ctx.table;

