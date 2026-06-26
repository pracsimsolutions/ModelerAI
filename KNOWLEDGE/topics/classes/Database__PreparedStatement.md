---
id: database__preparedstatement
name: "Database::PreparedStatement"
kind: class
module: "Database"
signature: "Database::PreparedStatement"
aliases: []
tags: ["class", "database-module"]
deprecated: false
replacedBy: null
description: "A class for execution and manipulation of a database SQL statement."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Database\\Database.PreparedStatement.xml"
---

# Database::PreparedStatement

A class for execution and manipulation of a database SQL statement.

The Database.PreparedStatement class is returned from the
Database.Connection.prepareStatement() method.
Calling this method creates a new PreparedStatement object. This object can be executed multiple times, and different values can be bound
to named or numbered parameters in the statement.

Database.Connection con = Database.Connection("DBConnector1");
con.connect();
Database.PreparedStatement statement = con.prepareStatement("INSERT INTO customers (ID, FirstName, LastName) VALUES (:id, :firstName, :lastName)");
Table customers = Table("Customers");
for (int i = 1; i <= customers.numRows; i++) {
statement.bindParam("id", customers[i]["ID"], Database.DataType.Int);
statement.bindParam("firstName", customers[i]["FirstName"], Database.DataType.VarChar);
statement.bindParam("lastName", customers[i]["LastName"], Database.DataType.VarChar);
statement.execute();
}
con.disconnect();

## Methods

### bindParam

```flexscript
void Database::PreparedStatement.bindParam(string paramName, Variant value, int dataType)
```

Binds a value to a named or numbered parameter in the SQL statement.

**Parameters:**
- `paramName` *(string)* — The name of a parameter defined in the SQL statement.
- `value` *(Variant)* — The value to bind the parameter to.
- `dataType` *(int)* — The database's storage data type for that parameter. This should be one of the values defined in Database.DataType.

To define a parameter for binding, add to the SQL statement a colon followed by either a
number or a named alias.
Below is an example of an SQL statement with named parameters.
INSERT INTO customers ID, FirstName, LastName) VALUES (:id, :firstName, :lastName)
In this example, you would bind the parameter by its name.
statement.bindParam("id", myID);
Below is an example of an SQL statement with numbered parameters.
INSERT INTO customers ID, FirstName, LastName) VALUES (:1, :2, :3)
In this example, you would bind the parameter by its number.
statement.bindParam(1, myID);

### execute

```flexscript
Database.ResultSet Database::PreparedStatement.execute()
```

Executes the statement with the currently bound parameter values.

**Returns:** The ResultSet associated with this execution.

In the current
implementation, only one ResultSet is available at any given time for a given
PreparedStatement. This means that you must completely process a result set before
calling execute() again. Subsequent calls to execute() will cause any previously
generated ResultSets to refer to the new result of the execute() call.

## Properties

### statement

```flexscript
string Database::PreparedStatement.statement (readonly)
```

The SQL statement associated with this PreparedStatement.

