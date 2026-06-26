---
id: agv__connections
name: "AGV::Connections"
kind: class
module: "AGV"
signature: "AGV::Connections"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "An interface for accessing AGV control point connections."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.Connections.xml"
---

# AGV::Connections

An interface for accessing AGV control point connections.

The AGV.Connections class allows you to query and dereference control point connections.
You construct an AGV.Connections instance with a constructor. Here you pass the owner of the connections,
followed by the name of the connection.
AGV.Connections connections = AGV.Connections(current, "Location");
You then can query how many connections are present with the length property.
int numConnections = connections.length;
To dereference a specific connection, use the [] array dereference operator.
Object obj = connections[1];

## Methods

### Connections

```flexscript
AGV::Connections.Connections(Object owner, string connectionName)
```

Creates a new set of connections with the passed owner and associated with the named connection.

**Parameters:**
- `owner` *(Object)* — The owner of the control point connections.
- `connectionName` *(string)* — The name of the control point connections.

## Properties

### length

```flexscript
int AGV::Connections.length (readonly)
```

Returns the number of connections present on the object.

int numConnections = AGV.Connections(current, "PickupPoints").length;

### rank

```flexscript
int AGV::Connections.rank (readonly)
```

Gets the rank of the connection type.

This is for use in subsequent AGV.Connections() constructors.

