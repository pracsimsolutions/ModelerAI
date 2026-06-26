---
id: storage__object
name: "Storage::Object"
kind: class
module: "Storage"
signature: "Storage::Object"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A storage object, with functionality and data for storing items in a 2-dimensional grid of bays, levels, and slots"
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Object.xml"
---

# Storage::Object

A storage object, with functionality and data for storing items in a 2-dimensional grid of bays, levels, and slots

## Methods

### areSlotLabelsEqual

```flexscript
int Storage::Object.areSlotLabelsEqual(string labelName)
```

Returns 1 if all slot label values are equal to each other or to a defined value, otherwise 0.

**Parameters:**
- `labelName` *(string)* — The name of the target label for matching.

**Returns:** If all slot label values match, then 1, otherwise 0.

### assignSlot

```flexscript
Storage.Slot Storage::Object.assignSlot(treenode item)
```

Assigns the item a slot, using the object's slot assignment strategy.

**Parameters:**
- `item` *(treenode)* — The item you want to assign a slot to.

**Returns:** The assigned slot if successful, null otherwise.

### getSlot

```flexscript
Storage.Slot Storage::Object.getSlot(Variant bayID, Variant levelID, Variant slotID)
```

Gets a slot by its bay, level, and slot ID.

**Parameters:**
- `bayID` *(Variant)* — The ID of the bay. If the object does not have an Address Scheme, then the bay ID should be a number. Otherwise it may be a letter if the address scheme's bay identifier is a letter.
- `levelID` *(Variant)* — The ID of the level. If the object does not have an Address Scheme, then the level ID should be a number. Otherwise it may be a letter if the address scheme's level identifier is a letter.
- `slotID` *(Variant)* — The ID of the slot. If the object does not have an Address Scheme, then the slot ID should be a number. Otherwise it may be a letter if the address scheme's slot identifier is a letter.

**Returns:** If the slot exists, the method returns that slot, otherwise NULL.

IDs are "mapped" in that they use the object's
Start Bay/Level/Slot and
Bay/Level/Slot Stride
properties. For example, if an object has a start bay of 2, and a bay stride of 2, then passing 6 as bayID
will retrieve the object's 3rd ranked bay (the 1st ranked bay is ID 2, the 2nd ranked bay is ID 4, the 3rd ranked
bay is ID 6, etc.).

Example:
Storage.Item(item).assignedSlot = current.getSlot(item.BayID, item.LevelID, item.SlotID)
This code assigns an item to the slot addressed by the item's "BayID", "LevelID", and "SlotID" labels.

Example:
Storage.Item(item).assignedSlot = current.getSlot(5, "A", 2)
This code assigns an item to the slot addressed by bayID 5, levelID "A", and slotID 2.

### setSlotLabels

```flexscript
void Storage::Object.setSlotLabels(string labelName, Variant value)
```

Sets a label on all slots of all bays and levels of the object to the given value.

**Parameters:**
- `labelName` *(string)* — The name of the label to set.
- `value` *(Variant)* — The value to give the label on each slot.

If the label does not exist on any of the slots, this method will add the
label and set the value.

## Properties

### bays

```flexscript
Storage::Object.bays (readonly)
```

Accesses the object's bays as an array of Storage.Bays.

Accessing the Number of Bays
You can access the number of bays with the following code.
current.bays.length
Accessing Bays by Rank
You can access an individual bay by rank with the following code.
current.bays[1] // first bay
current.bays[current.bays.length] // last bay
current.bays[i] // i-th bay
Bay Progression
Bays progress according the object's
Bay Progression
property. If the bay progression is X+, then
current.bays[1] accesses the bay with the smallest x location in object coordinates. For example,
this would be the left-most bay when facing a rack's
pick face. If the bay progression is X-, then current.bays[1] accesses the bay with the largest x location in
object coordinates. This would be the right-most bay when facing a rack from its pick face.

