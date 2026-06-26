---
id: database__connection
name: "Database::Connection"
kind: class
module: "Database"
signature: "Database::Connection"
aliases: []
tags: ["class", "database-module"]
deprecated: false
replacedBy: null
description: "A class for connecting to and querying a database."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Database\\Database.Connection.xml"
---

# Database::Connection

A class for connecting to and querying a database.

Every database connector that you create in your model's toolbox will have
a Database.Connection associated with it. To access the connection associated with a connector, use the
name-based constructor, providing the name of the database connector.

Database.Connection con = Database.Connection("DBConnector1");

## Methods

### connect

```flexscript
int Database::Connection.connect()
```

Attempts to connect to the database.

**Returns:** 1 if successful, 0 otherwise.

### Database.Connection

```flexscript
Database::Connection.Database.Connection(string connectorName)
```

Retrieves the connection associated with a named database connector.

**Parameters:**
- `connectorName` *(string)* — The name of the connector whose connection should be retrieved.

Database.Connection con = Database.Connection("DBConnector1");

### disconnect

```flexscript
void Database::Connection.disconnect()
```

Disconnects from the database.

### prepareStatement

```flexscript
Database.PreparedStatement Database::Connection.prepareStatement(string sqlStatement)
```

Creates a new Database.PreparedStatement object for executing the SQL statement.

**Parameters:**
- `sqlStatement` *(string)* — The SQL statement to be executed by the Database.PreparedStatement.

**Returns:** A newly created Database.PreparedStatement.

Prepared statements are bound to the tree. A connection will automatically remove all
its prepared statements when it is disconnected.

### query

```flexscript
Database.ResultSet Database::Connection.query(string sqlQuery)
```

Executes a SQL query, returning the result.

**Parameters:**
- `sqlQuery` *(string)* — The SQL query to execute.

## Properties

### isConnected

```flexscript
int Database::Connection.isConnected (readonly)
```

1 if the connection is currently connected, 0 otherwise.

