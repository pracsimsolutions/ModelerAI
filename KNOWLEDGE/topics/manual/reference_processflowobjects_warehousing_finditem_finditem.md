---
id: reference_processflowobjects_warehousing_finditem_finditem
name: "Find Item"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Warehousing","FindItem","FindItem"]
tags: ["manual","reference","processflowobjects","warehousing"]
source: "manual/Reference/ProcessFlowObjects/Warehousing/FindItem/FindItem.html"
---

# Find Item

## Overview and Key Concepts

 

The Find Item activity queries the model's Storage System for items stored in warehousing objects.

 

 

Queries will be executed one at a time, in order from top to bottom. These queries use SQL syntax. Each query can have up to five dynamic parameters passed into it using the $ syntax. These five parameters are the same for all queries in this activity. They can refer to a token, 3D object or other values. For example a query might look like:

 

```
WHERE item.slot.bayID == $1.bay
```

 

With a $1 parameter of:

 

```
token
```

 

This query tries to match the item's current bay to the bay label on the token. If a query finds a matching item, the activity will stop evaluating queries and the **On Success** trigger code will fire. This can be used to determine which query was successful, set labels or perform other logic.

 

The Find Item activity will then assign the found item to a label that can be used in later activities. This item referenced is the 3D flowitem. To get a reference to the Storage.Item, use the following code:

 

```
Storage.Item storageItem = Storage.Item(token.item);
```

 

It is not required to assign the item to a label. You may just want to find the item and mark it as ready to leave the storage object. This can be done with the **Mark Item as Outbound** property.

 

By default, the token will wait in the Find Item activity until an item is found. The queries will be reevaluated each time a new item enters the storage system. You can specify a **Max Wait Timer** and release a token after a specified amount of time if it has not found an item.

 

 

 

## Connectors

 

The Find Item activity allows multiple connectors out of the activity. However, tokens automatically released from this activity will be released through the first connector. Only manually released tokens can exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Find Item activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Query

 

Define any number of queries to find an item in the model's Storage System. This takes the form of an SQL query. You can use the button for help in constructing this query.

 

### On Success

 

The On Success trigger will be evaluated if its associated **Query** is successful in finding an item.

 

### Parameters

 

The parameters panel is used to define dynamic values that are passed into the queries. Use $1, $2, $3, $4 and $5 in a query to reference the parameter value. Parameters are not required.

 

### Assign To

 

Assigns a reference on the specified label/node to the item that was found in the Storage System. See the Assign To section of Common Properties for more information.

 

### FIFO

 

If checked, the First In First Out rule will ensure that the order tokens arrive at the Find Item activity is the same order they will leave.

 

### Mark Item as Outbound

 

If checked, the assigned slot of the found item is cleared, and its state becomes outbound, signaling that the item is about to exit the storage object. Items that are marked as outbound will be ignored in subsequent item queries, ensuring that two unassociated processes don't lock onto the same item.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not found an item after the specified time. See Max Wait Timer for more information.
