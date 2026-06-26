---
id: list__backorder
name: "List::BackOrder"
kind: class
module: "List"
signature: "List::BackOrder"
aliases: []
tags: ["class", "list-module"]
deprecated: false
replacedBy: null
description: "A class that represents a list back order."
seeAlso: []
source: "help\\FlexScriptAPIReference\\List\\List.BackOrder.xml"
---

# List::BackOrder

A class that represents a list back order.

List back orders represent pull requests that are waiting to be fulfilled.

## Methods

### remove

```flexscript
void List::BackOrder.remove()
```

Removes the back order from the partition.

List("ItemList").backOrders()[1].remove();
Use this instead of destroyobject() so the list can keep track of its statistics.

## Properties

### flags

```flexscript
int List::BackOrder.flags (readonly)
```

The flags used by this back order.

int requireNum = List("ItemList").backOrders()[1].flags;

### labelProperties

```flexscript
Variant List::BackOrder.labelProperties
```

Use your own named properties to get/set label values.

Works just like labelProperties on treenodes.

### labels

```flexscript
List::BackOrder.labels (readonly)
```

Accesses a node's labels as nodes.

Works just like labels on treenodes.

### numRequested

```flexscript
double List::BackOrder.numRequested (readonly)
```

The number of things this back order is requesting.

double requestNum = List("ItemList").backOrders()[1].numRequested;

### numRequired

```flexscript
double List::BackOrder.numRequired (readonly)
```

The number of things this back order is requiring.

double requireNum = List("ItemList").backOrders()[1].numRequired;

### puller

```flexscript
Variant List::BackOrder.puller (readonly)
```

Gets the variant value of the thing pulling from the list.

List("ItemList").backOrders()[1].puller == current;

### query

```flexscript
string List::BackOrder.query (readonly)
```

Gets the sql query this back order is using to define the pull requirements.

List("ItemList").backOrders()[1].query.startsWith("WHERE");

