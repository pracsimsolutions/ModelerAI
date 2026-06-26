---
id: reference_processflowobjects_warehousing_findslot_findslot
name: "Find Slot"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Warehousing","FindSlot","FindSlot"]
tags: ["manual","reference","processflowobjects","warehousing"]
source: "manual/Reference/ProcessFlowObjects/Warehousing/FindSlot/FindSlot.html"
---

# Find Slot

## Overview and Key Concepts

 

The Find Slot activity queries the model's Storage System for slots in warehousing objects.

 

 

Queries will be executed one at a time, in order from top to bottom. These queries use SQL syntax. Each query can have up to five dynamic parameters passed into it using the $ syntax. These five parameters are the same for all queries in this activity. They can refer to a token, 3D object or other values. For example a query might look like:

 

```
WHERE Type == $1.Type
```

 

With a $1 parameter of:

 

```
token
```

 

This query accesses the *Type* label from the queried slots and tries to match it to the *Type* label on the token. If a query finds a matching item, the activity will stop evaluating queries and the **On Success** trigger code will fire. This can be used to determine which query was successful, set labels or perform other logic.

 

The Find Slot activity will then assign the found slot to a label that can be used in later activities. This slot is of type Storage.Slot. To get a reference to the Storage Object, use the following code:

 

```
Storage.Object storageObject = token.slot.as(Storage.Slot).storageObject;
```

 

```
Object rack = token.slot.as(Storage.Slot).storageObject;
```

 

It is not required to assign the slot to a label. You may want to immediately assign an item to the slot. This can be done with the **Assign Slot to Item** property. This should be a reference to a 3D flowitem. This property is not required.

 

By default, the token will wait in the Find Slot activity until a slot is found. The queries will be reevaluated each time an item exits the storage system, freeing up space in a slot. You can specify a **Max Wait Timer** and release a token after a specified amount of time if it has not found a slot.

 

 

 

## Connectors

 

The Find Slot activity allows multiple connectors out of the activity. However, tokens automatically released from this activity will be released through the first connector. Only manually released tokens can exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Find Slot activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Query

 

Define any number of queries to find a slot in the model's Storage System. This takes the form of an SQL query. You can use the button for help in constructing this query. Alternatively you can define an address based upon the Storage System address schemes to find a specific slot.

 

### On Success

 

The On Success trigger will be evaluated if its associated **Query** is successful.

 

### Parameters

 

The parameters panel is used to define dynamic values that are passed into the queries. Use $1, $2, $3, $4 and $5 in a query to reference the parameter value. Parameters are not required.

 

### Assign To

 

Assigns a reference on the specified label/node to the slot that was found from the Storage System. This slot is of class Storage.Slot. See the Assign To section of Common Properties for more information. This property is not required.

 

### Assign Slot to Item

 

Assigns the found slot from the Storage System to the defined item's assignedSlot property. The item may then be moved into the slot's storage object either immediately or through some transit process. Once the item is moved into the storage object, its assigned slot will become its current slot. This property is not required.

 

### FIFO

 

If checked, the First In First Out rule will ensure that the order tokens arrive at the Find Slot activity is the same order they will leave.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not found a slot after the specified time. See Max Wait Timer for more information.
