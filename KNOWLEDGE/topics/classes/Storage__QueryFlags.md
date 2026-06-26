---
id: storage__queryflags
name: "Storage::QueryFlags"
kind: class
module: "Storage"
signature: "Storage::QueryFlags"
aliases: []
tags: ["class", "storage-module"]
deprecated: false
replacedBy: null
description: "Enumerates possible flags that can be passed as the flags parameter into Storage.System.findSlot(), Storage.System.querySlots(), Storage.System.findItem(), or Storage.System.queryItems()."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Warehousing\\Storage.QueryFlags.xml"
---

# Storage::QueryFlags

Enumerates possible flags that can be passed as the flags parameter into Storage.System.findSlot(), Storage.System.querySlots(), Storage.System.findItem(), or Storage.System.queryItems().

By default Storage.System.findSlot(),
and Storage.System.querySlots() will
only look at storable slots. Similarly,
Storage.System.findItem(),
and Storage.System.queryItems()
will exclude all inbound and outbound items by default. These flags allow you to better customize
which slots/items should be queried.

Each flag is a bit field, meaning different flags can be bitwise-ORed together to
include/exclude the desired slots/items.

## Properties

### AllItems

```flexscript
int Storage::QueryFlags.AllItems (readonly)
```

Valid only in findItem() and queryItems(). Signifies that all items should be included in the query. This value is a the same as QueryFlags.InboundItems | QueryFlags.OutboundItems.

### AllSlots

```flexscript
int Storage::QueryFlags.AllSlots (readonly)
```

Valid only in findSlot() and querySlots(). Signifies that all slots should be included in the query. This flag is the same as UnstorableSlots.

### ExcludeStorableSlots

```flexscript
int Storage::QueryFlags.ExcludeStorableSlots (readonly)
```

Valid only in findSlot() and querySlots(). Signifies that storable slots should be excluded from the query.

### ExcludeStoredItems

```flexscript
int Storage::QueryFlags.ExcludeStoredItems (readonly)
```

Valid only in findItem() and queryItems(). Signifies that stored items (items that are neither inbound nor outbound) should be excluded from the query.

### InboundItems

```flexscript
int Storage::QueryFlags.InboundItems (readonly)
```

Valid only in findItem() and queryItems(). Signifies that inbound items should be included in the query.

### Limit1

```flexscript
int Storage::QueryFlags.Limit1 (readonly)
```

Limits the results of querySlots() and queryItems() to a single result. findSlot() and findItem() forward to querySlots() and queryItems() respectively, automatically asserting this flag. As such, generally you do not need to use this flag, because you can just call findSlot() or findItem().

### OutboundItems

```flexscript
int Storage::QueryFlags.OutboundItems (readonly)
```

Valid only in findItem() and queryItems(). Signifies that outbound items should be included in the query.

### UnstorableSlots

```flexscript
int Storage::QueryFlags.UnstorableSlots (readonly)
```

Valid only in findSlot() and querySlots(). Signifies that unstorable slots should be included in the query.

