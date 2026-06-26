---
id: storage__level
name: "Storage::Level"
kind: class
module: "Storage"
signature: "Storage::Level"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A class that represents a storage level within a bay."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Level.xml"
---

# Storage::Level

A class that represents a storage level within a bay.

A Level is defined as the area of a Bay above the floor
or above a horizontal support.

## Methods

### areSlotLabelsEqual

```flexscript
int Storage::Level.areSlotLabelsEqual(string labelName)
```

Returns whether a label value is the same for all slots within a level.

**Parameters:**
- `labelName` *(string)* — The name of the label to check.

**Returns:** alse if a label value is not the same for all slots within the level, or if any of those values do not match the matchValue. Returns true otherwise.

The specified label value is checked for each slot of the level.
If all label values do not match, then this method returns false. If a matchValue
is provided, all label values must match the matchValue, or this function
will still return false.

### setSlotLabels

```flexscript
void Storage::Level.setSlotLabels(string labelName, Variant value)
```

Sets a label on all slots of the level to the given value.

**Parameters:**
- `labelName` *(string)* — The name of the label to set.
- `value` *(Variant)* — The value to give the label on each slot.

If the label does not exist on any of the slots, this method will add the
label and set the value.

## Properties

### slots

```flexscript
Storage::Level.slots (readonly)
```

Accesses the level's slots as an array of Storage.Slots.

