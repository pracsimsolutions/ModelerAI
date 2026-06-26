---
id: storage__system
name: "Storage::System"
kind: class
module: "Storage"
signature: "Storage::System"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A class that represents a storage system."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.System.xml"
---

# Storage::System

A class that represents a storage system.

For more information on how to use the storage
system, see the
Storage System
topic.

## Methods

### findItem

```flexscript
Storage.Slot.Item Storage::System.findItem(Variant queries, int flags)
```

Finds an item matching one of the given queries.

**Parameters:**
- `queries` *(Variant)* — One or more queries to use to find an item. One query can be passed in as a string. Multiple must be passed in an array.
- `flags` *(int)* — A bit field that may include Storage.QueryFlags values.

**Returns:** A reference to the first matching slot item. If the search was unsuccesful, this value will be empty.

This method evaluates each query until a slot item
is found that matches. Only stored items are searched.

'$' Keywords
In composing the query, you can include the values $1 - $5. These values are associated with additional
parameters that you pass into the query.
Storage.system.findItem("WHERE SKU = $1 AND Size = $2", 0, token.SKU, token.Size);
'item' Keyword
The query parser will interpret 'item' is a special keyword that accesses the
target item that is being evaluated, as a Storage.Slot.Item.
This can be used to write normal flexscript expressions as part of the query.
Storage.system.findItem("WHERE item.slot.SKU = $1", 0, token.SKU);
Accessing Label Names
Other than the 'item' keyword and the '$' keywords, all other values in the query are interpreted
to be labels on the evaluating flow item.
For example, to search for items that have a Type label value
of 5, you could use the following code:
Storage.Slot.Item slotItem = Storage.system.findItem("WHERE Type = 5");
If you are virtualizing items, then you should make sure those
items are included in the
list of item labels to save.
Speed Considerations
It should be noted that evaluating a label directly by its name effects a result no different than accessing
it via the 'item' keyword. In other words, the two queries below will give the same result (assuming you are
not virtualizing items).
WHERE SKU = 5
WHERE item.item.SKU = 5
However, these two expressions can have different results in speed, especially when searching large
storage systems. Accessing the SKU label directly (the first query above) will enable indexed lookup if
you have indexed that label.
On the other hand, using the expression 'item.item.SKU' will compile to a straight flexscript evaluation,
which cannot be indexed. Thus, using the direct label access is preferable.

### findSlot

```flexscript
Storage.Slot Storage::System.findSlot(Variant queries, int flags)
```

Finds a slot that matches one of the given queries.

**Parameters:**
- `queries` *(Variant)* — One or more queries to use to find a slot. One query can be passed in as a string. Multiple must be passed in an array.
- `flags` *(int)* — A bit field that may include Storage.QueryFlags values.

**Returns:** A reference to the first matching slot. If the search was unsuccesful, this value will be empty.

This method evaluates each query until a slot
is found that matches. Only storable slots are searched.

'$' Keywords
In composing the query, you can include the values $1 - $5. These values are associated with additional
parameters that you pass into the query.
Storage.system.findSlot("WHERE SKU = $1 AND Size = $2", 0, token.SKU, token.Size);
'slot' Keyword
The query parser will interpret 'slot' as a special keyword that accesses the
target slot that is being evaluated, as a Storage.Slot.
This can be used to write normal flexscript expressions as part of the query.
Storage.system.findItem("WHERE slot.hasSpace($1)", 0, token.Item);
Accessing Label Names
Other than the 'slot' keyword and the '$' keywords, all other values in the query are interpreted
to be labels on the evaluating slot. For example, to search for slots that have a Type label value
of 5, you could use the following code:
Storage.Slot.Item slotItem = Storage.system.findItem("WHERE Type = 5");
Speed Considerations
It should be noted that evaluating a label directly by its name effects a result no different than accessing
it via the 'slot' keyword. In other words, the two queries below will give the same result.
WHERE SKU = 5
WHERE slot.SKU = 5
However, these two expressions can have different results in speed, especially when searching large
storage systems. Accessing the SKU label directly (the first query above) will enable indexed lookup if
you have indexed that label.
On the other hand, using the expression 'slot.SKU' will compile to a straight flexscript evaluation,
which cannot be indexed. Thus, using the direct label access is preferable. Indeed, in some cases
it may be advantageous to avoid using the 'slot' keyword, and instead store desired values on labels on the
slot, specifically to take advantage of indexed lookup.

### getSlot

```flexscript
Storage.Slot Storage::System.getSlot(string address)
```

Gets a slot by its address.

**Parameters:**
- `address` *(string)* — An address that can be interpreted by one of the address schemes.

This method compares the address to each
Address Scheme. If it matches, this method then
tries to resolve the slot using the scheme.

### queryItems

```flexscript
Variant Storage::System.queryItems(string query, int flags)
```

Searches for all items that match the query.

**Parameters:**
- `query` *(string)* — The query to run.
- `flags` *(int)* — A bit field that may include Storage.QueryFlags values.

**Returns:** An array of Storage.Slot.Item values.

The method only considers items that are in the
Stored state.

### querySlots

```flexscript
Variant Storage::System.querySlots(string query, int flags)
```

Searches for all slots that match a query.

**Parameters:**
- `query` *(string)* — The query to run
- `flags` *(int)* — A bit field that may include Storage.QueryFlags values.

**Returns:** An array of Storage.Slot values.

This method only considers storable slots.

## Properties

### storageObjects

```flexscript
Storage::System.storageObjects (readonly)
```

Accesses all storage objects in the model as an array of Storage.Objects.

