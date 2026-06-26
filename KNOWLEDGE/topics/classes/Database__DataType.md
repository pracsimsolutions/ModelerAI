---
id: database__datatype
name: "Database::DataType"
kind: class
module: "Database"
signature: "Database::DataType"
aliases: []
tags: ["class", "database-module"]
deprecated: false
replacedBy: null
description: "A class that enumerates various database storage types."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Database\\Database.DataType.xml"
---

# Database::DataType

A class that enumerates various database storage types.

This class' enumerations are used in binding parameters in a
Database.PreparedStatement.

statement.bindParam(1, myValue, Database.DataType.SmallInt);

This set of available values corresponds closely with the ANSI standard
SQL language's set of supported database storage types.

## Methods

### fromString

```flexscript
int Database::DataType.fromString(string typeName)
```

Gets the data type value associated with the name of a storage type.

**Parameters:**
- `typeName` *(string)*

**Returns:** The data type value for the given name.

This can be used if you are querying the information schema of a database. In
information schemas, field storage types are stored as strings such as "varchar" or "smallint".
This method will interpret such a string an return the enumeration type value associated with it.

## Properties

### BigInt

```flexscript
int Database::DataType.BigInt (readonly)
```

A 64-bit integer.

### Bool

```flexscript
int Database::DataType.Bool (readonly)
```

A boolean 0 or 1 value.

### Char

```flexscript
int Database::DataType.Char (readonly)
```

A fixed length string.

### Date

```flexscript
int Database::DataType.Date (readonly)
```

A date value.

### DateTime

```flexscript
int Database::DataType.DateTime (readonly)
```

A date-time value.

### Decimal

```flexscript
int Database::DataType.Decimal (readonly)
```

A decimal value.

### Double

```flexscript
int Database::DataType.Double (readonly)
```

A double precision floating point value.

### Float

```flexscript
int Database::DataType.Float (readonly)
```

A variable precision floating point value.

### Int

```flexscript
int Database::DataType.Int (readonly)
```

A 32-bit integer generally 32-bit.

### Interval

```flexscript
int Database::DataType.Interval (readonly)
```

A time interval value.

### NChar

```flexscript
int Database::DataType.NChar (readonly)
```

A fixed length unicode string.

### Numeric

```flexscript
int Database::DataType.Numeric (readonly)
```

A numeric value.

### NVarChar

```flexscript
int Database::DataType.NVarChar (readonly)
```

A variable length unicode string.

### Real

```flexscript
int Database::DataType.Real (readonly)
```

A 32-bit floating point value.

### SmallInt

```flexscript
int Database::DataType.SmallInt (readonly)
```

A 16-bit integer.

### Text

```flexscript
int Database::DataType.Text (readonly)
```

A variable length string.

### Time

```flexscript
int Database::DataType.Time (readonly)
```

A time value.

### Unknown

```flexscript
int Database::DataType.Unknown (readonly)
```

An unknown data type.

### VarChar

```flexscript
int Database::DataType.VarChar (readonly)
```

A variable length string.

