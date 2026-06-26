---
id: storage__item
name: "Storage::Item"
kind: class
module: "Storage"
signature: "Storage::Item"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A class that includes properties and functionality associated with the storage and assignment of a flow item to Storage.Slots."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Item.xml"
---

# Storage::Item

A class that includes properties and functionality associated with the storage and assignment of a flow item to Storage.Slots.

The Storage.Item class is primarily used for managing and retrieving the slots that a flow item is assigned to
and/or stored in.
The Storage.Item is NOT the same as the item object. In other words, you can't just set a Storage.Item to the item itself.
Object item = param(2);
Storage.Item storageItem = item; // RUN-TIME ERROR
Instead, the Storage.Item is associated with, not the same as the item. To access the Storage.Item of
a given flow item, use the
Storage.Item() constructor.

A Storage.Item holds data associated directly with the flow item itself. A Storage.Item should not be confused with
a Storage.Slot.Item. The Storage.Item may have both a current slot
(the slot that the flow item is currently stored in), and/or an assigned slot (the slot
that the item has been assigned to be placed in). If the flow item has been placed in its assigned slot, and has not been
assigned to any new slot, then its assigned slot is the same as its current slot.
A Storage.Item may simultaneously have two associated Storage.Slot.Items, namely
one for its assigned slot, and one for its current slot. These can be accessed respectively with the
assignedSlotItem and
currentSlotItem properties.

## Methods

### Storage.Item

```flexscript
Storage.Item Storage::Item.Storage.Item(treenode item)
```

Gets/Asserts the Storage.Item object associated with the given flow item.

**Parameters:**
- `item` *(treenode)* — A flow item.

**Returns:** A Storage.Item object. If an associated Storage.Item does not already exist for the flow item, it will be automatically created.

A Storage.Item can only be accessed by calling this method. Casting from a treenode is not correct.
Storage.Item storageItem = Storage.Item(item); // correct
Storage.Item storageItem = item; // incorrect

## Properties

### assignedSlot

```flexscript
Storage.Slot Storage::Item.assignedSlot
```

The item's assigned slot.

You can set this property to a specific slot through several methods:

Find a slot in the storage system using
Storage.System.findSlot(), and set
assignedSlot to the result.
Get a slot directly by its address, using
Storage.System.getSlot(), and
set assignedSlot to the result.
Get a slot directly by its bay, level, and slot ID, using
Storage.Object.getSlot(), and set
assignedSlot to the result.
Search through a Storage.Object's bays, levels, and slots to find an
appropriate slot, and set assignedSlot to the found slot.
Directly set assignedSlot to NULL. This will put the item in an outbound
state. Item searches using
Storage.system.findItem() will
skip over outbound items by default.

Once you have assigned the item to a slot, move the item into the slot's storage object, either
immediately or through some transit process. Once the item is moved into the storage object, its assigned
slot will become its current slot.

### assignedSlotItem

```flexscript
Storage.Slot.Item Storage::Item.assignedSlotItem (readonly)
```

The Storage.Slot.Item associated with the item's assigned slot.

### currentSlot

```flexscript
Storage.Slot Storage::Item.currentSlot (readonly)
```

The item's current slot.

The item's current slot is the slot that the item is currently stored in. You cannot directly set
the item's current slot. Instead, you can do one of the following:

Directly set the item's assignedSlot
and then move the flow item (either immediately or through some transit process) into that slot's
storage object. Here when the item is moved into the storage object, the assigned slot will be
recognized and its assigned slot will become its current slot.
Leave the assignedSlot property unassigned, and
just move the flow item into a storage object. When the storage object receives the item, it will
recognized that the item has no assigned slot, and will fire its slot assignment strategy, which
should assign the slot. Then the item's assigned slot will become its current slot.

### currentSlotItem

```flexscript
Storage.Slot.Item Storage::Item.currentSlotItem (readonly)
```

The Storage.Slot.Item associated with the item's current slot.

### state

```flexscript
int Storage::Item.state (readonly)
```

The current transit state of the item.

The state value is a function of the item's currentSlot
and assignedSlot property values.
Valid values are:

Storage.Item.State.Unassigned
Storage.Item.State.Inbound
Storage.Item.State.Stored
Storage.Item.State.Outbound

