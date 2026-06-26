---
id: database__resultset
name: "Database::ResultSet"
kind: class
module: "Database"
signature: "Database::ResultSet"
aliases: []
tags: ["class", "database-module"]
deprecated: false
replacedBy: null
description: "A class for retrieving the result of a database SQL query."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Database\\Database.ResultSet.xml"
---

# Database::ResultSet

A class for retrieving the result of a database SQL query.

A Database.ResultSet is returned by
Database.Connection.query(), and by
Database.PreparedStatement.execute().
The class provides methods and properties for retrieving the result data associated with a SQL query execution.

Database.Connection con = Database.Connection("DBConnector1");
con.connect();
Database.ResultSet result = con.query("SELECT * FROM customers");
while (result.fetchNext()) {
string firstName = result["FirstName"];
string lastName = result["LastName"];
for (int j = 1; j <= result.numFields; j++) {
Variant column1Value = result[j];
...
}
...
}
con.disconnect();

A Database.ResultSet implements a forward-only cursor. This means you can only traverse through the results once.
This can either be done manually, using fetchNext(), or automatically
by dumping the data to a table with cloneTo(). However, you cannot do
both. In other words, you cannot clone the result using cloneTo(), and then also use fetchNext() to traverse the result again,
or vice versa.

## Methods

### cloneTo

```flexscript
void Database::ResultSet.cloneTo(Table destTable)
```

Clones the result set to a destination table.

**Parameters:**
- `destTable` *(Table)*

Database.Connection con = Database.Connection("DBConnector1");
con.connect();
con.query("SELECT * FROM customers").cloneTo(Table("Customers"));
con.disconnect();

### fetchNext

```flexscript
int Database::ResultSet.fetchNext()
```

Moves the cursor forward one record in the result.

**Returns:** 1 if successful, 0 if the end of the result set has been reached.

### getFieldName

```flexscript
string Database::ResultSet.getFieldName(int fieldNum)
```

Gets the name of a result column by index.

**Parameters:**
- `fieldNum` *(int)* — The index of the field.

**Returns:** The name of the field at the specified index.

Array fields;
Database.Connection db = Database.Connection("DatabaseConnector1");
db.connect();
var resultSet = db.query("SELECT * FROM customers");
for (int i = 1; i <= resultSet.numFields; i++) {
fields.push(resultSet.getFieldName(i));
}
db.disconnect();
return fields;

## Properties

### numFields

```flexscript
int Database::ResultSet.numFields (readonly)
```

The number of fields in the result set.

