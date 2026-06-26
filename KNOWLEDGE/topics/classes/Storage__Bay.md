---
id: storage__bay
name: "Storage::Bay"
kind: class
module: "Storage"
signature: "Storage::Bay"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A class that represents a storage bay."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Bay.xml"
---

# Storage::Bay

A class that represents a storage bay.

A Bay is defined as the area between to vertical supports,
and includes all levels between those two supports.

## Methods

### areSlotLabelsEqual

```flexscript
int Storage::Bay.areSlotLabelsEqual(string labelName)
```

Returns whether a label value is the same for all slots on all levels within a bay.

**Parameters:**
- `labelName` *(string)* — The name of the label to check

**Returns:** False if a label value is not the same for all slots within the bay, or if any of those values do not match the matchValue. Returns true otherwise.

The specified label value is checked for each slot of the bay.
If all label values do not match, then this method returns false. If a matchValue
is provided, all label values must match the matchValue, or this function
will still return false.

### setSlotLabels

```flexscript
void Storage::Bay.setSlotLabels(string labelName, Variant value)
```

Sets a label on all slots on all levels of the bay to the given value.

**Parameters:**
- `labelName` *(string)* — The name of the label to set.
- `value` *(Variant)* — The value to give the label on each slot.

If the label does not exist on any of the slots, this method will add the
label and set the value.

## Properties

### levels

```flexscript
Storage::Bay.levels (readonly)
```

Accesses the bay's levels as an array of Storage.Levels.

