---
id: storage__item.state
name: "Storage::Item.State"
kind: class
module: "Storage"
signature: "Storage::Item.State"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "A class that enumerates the storage item states."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.Item.State.xml"
---

# Storage::Item.State

A class that enumerates the storage item states.

## Properties

### Inbound

```flexscript
int Storage::Item.State.Inbound (readonly)
```

Storage item Inbound state.

The assignedSlot property is set to a valid slot, but the currentSlot property
is null. In other words, the item has been assigned to a slot, but has not yet arrived
in the slot.

### Outbound

```flexscript
int Storage::Item.State.Outbound (readonly)
```

Storage item Outbound state.

The currentSlot property refers to a valid slot, but the assignedSlot property
refers to different slot or null. In other words, the item has been stored in a slot,
and has been reassigned, either to a different slot, or to null (it is about to exit
the system).

### Stored

```flexscript
int Storage::Item.State.Stored (readonly)
```

Storage item Stored state.

The assignedSlot and currentSlot properties are both set to the same slot. In
other words, the item has been stored in a slot, and has not been assigned to any other
slot.

### Unassigned

```flexscript
int Storage::Item.State.Unassigned (readonly)
```

Storage item Unassigned state.

Both the assignedSlot and currentSlot properties are null. In other words, the
item is neither assigned to nor stored in a slot.

