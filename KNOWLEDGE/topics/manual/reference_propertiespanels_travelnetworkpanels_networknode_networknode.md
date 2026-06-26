---
id: reference_propertiespanels_travelnetworkpanels_networknode_networknode
name: "Network Node"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","TravelNetworkPanels","NetworkNode","NetworkNode"]
tags: ["manual","reference","propertiespanels","travelnetworkpanels"]
source: "manual/Reference/PropertiesPanels/TravelNetworkPanels/NetworkNode/NetworkNode.html"
---

# Network Node

The Network Node panel has options for controlling the Network Node's connections.

 

 

The following properties are on the Network Node panel:

 

### Max Travelers

 

This number defines how many transporters that are not traveling on the network can be stationed at the node. This would represent the transporters that are not currently executing a travel task, but are doing other things while "stationed" at the node.

 

### Side Offset

 

This number defines a distance to the right of outgoing paths that travelers will be offset. It does not affect the distance that the traveler travels, but is purely for visual purposes, so travelers going in different directions along the same path don't run over each other.

  

#### Network Node Connections

 

Each path between two network nodes contains two one-way connections. This page defines behavior only for the connections extending from this network node to other network nodes. If you would like to edit behavior for connections extending from other network nodes to this network node, then open the properties of those other nodes.

  

### Path

 

This combobox allows the user to select individual connections from this node to others and to name each connection. These names should be descriptive of any special purpose this connection has in the model.

 

### Connection Type

 

This drop-down list allows the user to define how this connection behaves. There are three options.

 

### No Connection

 

Transporters cannot travel on this connection. The connection is drawn in red in the view window.

 

### Passing

 

Transporters are allowed to pass each other on this connection. The connection is drawn in green in the view window.

 

### No Passing

 

Transporters will not pass each other on this connection. The minimum distance between transporters on the path can be set by the user in the Spacing field of the dialog. These connections are drawn in yellow in the view window.

 

### Spacing

 

This number determines the minimum distance allowed between two transporters on a connection that is designated as no passing. This is the distance from the back of one traveler to the front of the traveler behind it.

 

### Speed Limit

 

This number determines the maximum speed that a traveler can travel along this connection.

 

### Current Distance

 

This number shows you the current distance that is being simulated for that connection. If the virtual distance is specified as 0, then it will be the actual distance of the spline path. Otherwise it will be the distance that is specified in the virtual distance field.

 

### Virtual Distance

 

This number let's you specify an exact distance for the connection.
