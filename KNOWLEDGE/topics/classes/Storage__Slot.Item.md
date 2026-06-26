---
id: storage__slot.item
name: "Storage::Slot.Item"
kind: class
module: "Storage"
signature: "Storage::Slot.Item"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A class that represents a storage slot item."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Slot.Item.xml"
---

# Storage::Slot.Item

A class that represents a storage slot item.

A storage slot item is a record of an item that
is either assigned to the slot, or is currently in the slot.
If the item is virtual, the slot item holds the label values
of the item. A slot item is created when an item is assigned
to the slot. A slot item is destroyed when the item is no
longer associated with the slot, meaning the item is not assigned
to the slot, and it is not currently within the slot.

## Methods

### getLocation

```flexscript
Vec3 Storage::Slot.Item.getLocation(double xFactor, double yFactor, double zFactor)
```

Gets the item's location, relative to the storage object.

**Parameters:**
- `xFactor` *(double)* — The x value of the point on the item where the location will be measured from. Measured as a percent of the item's size (0-1).
- `yFactor` *(double)* — The y value of the point on the item where the location will be measured from. Measured as a percent of the item's size (0-1).
- `zFactor` *(double)* — The z value of the point on the item where the location will be measured from. Measured as a percent of the item's size (0-1).

**Returns:** A vector holding the item's location.

This code gets the item's location measured at its center.
Vec3 centerLoc = storageItem.getLocation(0.5,0.5,0.5);
Vec3 centerLoc = storageItem.getLocation(Vec3(0.5,0.5,0.5));

## Properties

### isVirtual

```flexscript
int Storage::Slot.Item.isVirtual (readonly)
```

Returns whether the current slot item is virtual. For more information on virtualization, see the Storage System topic.

### item

```flexscript
Object Storage::Slot.Item.item (readonly)
```

Returns the flowitem associated with this slot item. Accessing this property will cause a virtualized item to be re-created.

### labelProperties

```flexscript
Variant Storage::Slot.Item.labelProperties
```

Use your own named properties to get/set label values.

The Storage.Slot.Item class allows you to get and set custom named properties that will apply to labels on
the item. Accessing items on a Storage.Slot.Item acts as a "pass-through" to accessing labels on the item itself. In other
words, getting and setting labels on the Storage.Slot.Item is essentially the same as getting and setting labels directly
on the flowitem object.
The primary reason for accessing these labels via the Storage.Slot.Item is when the actual flowitem
is virtualized. When the flowitem is virtual, the
Storage.Slot.Item acts as its temporary store for labels. When the flowitem is later "unvirtualized" those labels will be
restored to the flowitem, with any intervening changes in the label values reflected in the restored flowitem.

### location

```flexscript
Vec3 Storage::Slot.Item.location (readonly)
```

Returns the location of the flowitem associated with this slot item, relative to the storage object.

### pickRank

```flexscript
int Storage::Slot.Item.pickRank (readonly)
```

Returns the order that this item could be picked. This value depends on the item insertion order as well as the rack type. For example, the last item in a Floor Storage has a pickRank of 1. By contrast, the first item in a Gravity Flow Rack would have a pickRank of 1.

### rotation

```flexscript
Vec3 Storage::Slot.Item.rotation (readonly)
```

Returns the rotation of the associated flowitem.

### size

```flexscript
Vec3 Storage::Slot.Item.size (readonly)
```

Retursn the size of the associated flowitem.

### slot

```flexscript
Storage.Slot Storage::Slot.Item.slot (readonly)
```

Returns the slot associated with this slot item.

### storageItem

```flexscript
Storage.Item Storage::Slot.Item.storageItem (readonly)
```

Returns the storage item for the flowitem associated with this slot item. Accessing this property will cause a virtualized item to be re-created.

