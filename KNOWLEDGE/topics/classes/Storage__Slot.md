---
id: storage__slot
name: "Storage::Slot"
kind: class
module: "Storage"
signature: "Storage::Slot"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A slot that is part of a storage object's grid of storable slots."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Slot.xml"
---

# Storage::Slot

A slot that is part of a storage object's grid of storable slots.

## Methods

### getLocation

```flexscript
Vec3 Storage::Slot.getLocation(double xFactor, double yFactor, double zFactor)
```

Gets a location within the slot.

**Parameters:**
- `xFactor` *(double)* — The normalized target x location within the slot. Should be in the range 0-1.
- `yFactor` *(double)* — The normalized target y location within the slot. Should be in the range 0-1.
- `zFactor` *(double)* — The normalized target z location within the slot. Should be in the range 0-1.

This method is similar to Object.getLocation(). It returns a slot-relative
location, in the storage object's local coordinates.

### hasSpace

```flexscript
int Storage::Slot.hasSpace(treenode item)
```

Returns 1 if it calculates that there is enough space to place the passed item into the slot, 0 otherwise.

**Parameters:**
- `item` *(treenode)* — The flow item that you want place into the slot.

**Returns:** 1 if the slot calculates that there is enough space in the slot to place the item, 0 otherwise.

The hasSpace() method uses the storage object's
Slot Stacking Order property to
"simulate" placing the item into the slot, taking into account the set of items that are already stored in or assigned to the slot.
Once it has resolved where the item would be placed if it were to be stored in the slot, it checks if that placement is fully
within the boundaries of the slot. If the placement is within the slot boundaries, the method will return 1, otherwise
the method will return 0.
Item Size and Rotation
The item's current size and rotation are used in calculating the item's placement. The item's size and rotation are assumed to
be the same as they will be when the item is eventually placed into the slot. This is important, especially for rotation, which
often changes during the model run. If, when the item is finally placed into the slot, the item's rotation is different than
when hasSpace() was called, then hasSpace()'s return value will have potentially been incorrect. To fix this, you should first set the
rotation of the item to the rotation it will be in when it is placed in the rack, then call hasSpace(). You can then set its rotation
back to whatever the item's current context requires.
Assigned-Only Items
When "simulating" the placement of the item to determine space availability, the slot will take into account not only the
items that are already stored in the slot, but also the items that are
assigned to the slot, but have not yet been stored in
the slot. The slot will first virtually "place" the assigned-only items into the slot, in the order
in which they were assigned to the slot. Then it will virtually place the currently querying item into the slot
as if it entered the slot after all the other assigned items.
Because of the virtual placement of assigned-only items, it is important to note that the hasSpace() method
is only a heuristic. The result of hasSpace() assumes that the order in which assigned items will be placed into
the slot is the same order as their assignment to the slot, and that the item's rotation and size are the same as
when it will be placed in the rack. If these factors change, or if different-sized items are placed into the
slot in a different order than their assignment order, then hasSpace() will have returned a value inconsistent
with the actually reality of whether there is space in the slot after all assigned items have been placed.

### restack

```flexscript
void Storage::Slot.restack(int fromRank = 0)
```

Performs again the slot stacking algorithm on some or all items stored in the slot.

**Parameters:**
- `fromRank` *(int, default 0)* — If non-zero, the slot will only start at the item at the defined rack, and restack from that point.

For storage objects whose Slot Stacking Order
only stacks along a single axis (and for Gravity Flow and Push Back Racks) the storage object will automatically restack when an item is picked
from a position that would require a restack. For slot stacking orders that include multiple axes, however, the storage object does not
automatically restack the slot when items are picked from a middle or front position. As such, you can manually restack the slot using this method.

## Properties

### address

```flexscript
string Storage::Slot.address (readonly)
```

The slot's address.

If the slot's storage object has an
Address Scheme,
the slot's address is formatted according to that address scheme. Otherwise, the address will be formatted as
follows: <StorageObjectName>-<BayNumber>-<LevelNumber>-<SlotNumber>.

### aisleID

```flexscript
Variant Storage::Slot.aisleID (readonly)
```

The slot's aisle ID.

If the slot's storage object has an
Address Scheme,
the slot's aisle ID is formatted according to that address scheme. In other words, if the address scheme's aisles
are addressed with a number, the aisle ID will be a number. Otherwise, the aisle ID will be a string.
If there is no address scheme, the aisle ID will be blank. All slots in a given Storage Object
use the Storage Object's aisle ID as their aisle ID.

### bay

```flexscript
Storage.Bay Storage::Slot.bay (readonly)
```

The slot's bay.

Returns the bay that this slot belongs to.

### bayID

```flexscript
Variant Storage::Slot.bayID (readonly)
```

The slot's bay ID.

If the slot's storage object has an
Address Scheme,
the slot's bay ID is formatted according to that address scheme. In other words, if the address scheme's bays
are addressed with a letter, bayID will be a string, otherwise bayID
will be a number. If there is no address scheme, bayID will be the bay number.
Bay IDs are mapped according to the storage object's
Start Bay and
Bay Stride.

### isStorable

```flexscript
int Storage::Slot.isStorable (readonly)
```

1 if the slot is a storable slot, 0 otherwise.

### labelProperties

```flexscript
Variant Storage::Slot.labelProperties
```

Use your own named properties to get/set slot label values.

The Storage.Slot class allows you to get and set custom named properties that will apply to labels on the slot. For example,
if you want to set a label named "SKU" on a given slot to a value of 5, you can write the following code:
slot.SKU = 5;
This will set the label's value to 5. If a label of that name does not exist, then it will be added to the slot's labels.
If you want to return the value stored in the slot's "SKU" label, you can execute the following code:
return slot.SKU;
For slot labels, you MUST access the label through this dynamic properties syntax. You should not use
syntax that accesses label nodes directly in the tree (like slot.labels.assert("SKU").value = 5;). The storage
system may manage an index of slot label values for quick lookup. This index is only updated properly when you use the
dynamic property syntax to set label values.

For more information, see treenode.labelProperties.

### level

```flexscript
Storage.Level Storage::Slot.level (readonly)
```

The slot's level.

Returns the level that this slot belongs to.

### levelID

```flexscript
Variant Storage::Slot.levelID (readonly)
```

The slot's level ID.

If the slot's storage object has an
Address Scheme,
the slot's level ID is formatted according to that address scheme. In other words, if the address scheme's levels
are addressed with a letter, levelID will be a string, otherwise levelID
will be a number. If there is no address scheme, levelID will be the level number.
Level IDs are mapped according to the storage object's
Start Level and
Level Stride.

### location

```flexscript
Vec3 Storage::Slot.location (readonly)
```

The location of the bottom front left corner of the slot, in the local coordinates of its storage object.

### size

```flexscript
Vec3 Storage::Slot.size (readonly)
```

The size of the slot.

### slotID

```flexscript
Variant Storage::Slot.slotID (readonly)
```

The slot's slot ID.

If the slot's storage object has an
Address Scheme,
the slot's slot ID is formatted according to that address scheme. In other words, if the address scheme's slots
are addressed with a letter, slotID will be a string, otherwise slotID
will be a number. If there is no address scheme, slotID will be the slot number.
Slot IDs are mapped according to the storage object's
Start Slot and
Slot Stride.

### slotItems

```flexscript
Storage::Slot.slotItems (readonly)
```

Accesses the array of Storage.Slot.Items that are stored in or assigned to this slot.

Accessing the Number of Slot Items
You can access the number of slot items with the following code.
slot.slotItems.length
Accessing Slot Items by Rank
You can access an individual slot item by rank with the following code.
slot.slotItems[1] // first slot item
slot.slotItems[slot.slotItems.length] // last slot item
slot.slotItems[i] // i-th slot item

### storageObject

```flexscript
Storage.Object Storage::Slot.storageObject (readonly)
```

The slot's storage object.

Returns the storage object that this slot belongs to.

### zoneID

```flexscript
Variant Storage::Slot.zoneID (readonly)
```

The slot's zone ID.

If the slot's storage object has an
Address Scheme,
the slot's zone ID is formatted according to that address scheme. In other words, if the address scheme's zones
are addressed with a number, the zone ID will be a number. Otherwise, the zone ID will be a string.
If there is no address scheme, the zone ID will be blank. All slots in a given Storage Object
use the Storage Object's zone ID as their zone ID.

