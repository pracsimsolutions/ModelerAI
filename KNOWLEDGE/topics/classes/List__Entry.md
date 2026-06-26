---
id: list__entry
name: "List::Entry"
kind: class
module: "List"
signature: "List::Entry"
aliases: []
tags: ["class", "list-module"]
deprecated: false
replacedBy: null
description: "A class that represents a list entry."
seeAlso: []
source: "help\\FlexScriptAPIReference\\List\\List.Entry.xml"
---

# List::Entry

A class that represents a list entry.

List entries represent values that have been pushed on the list and are waiting to be pulled.

## Methods

### remove

```flexscript
void List::Entry.remove()
```

Removes the entry from the partition.

List("ItemList").entries()[1].remove();
Use this instead of destroyobject() so the list can keep track of its statistics.

## Properties

### value

```flexscript
Variant List::Entry.value (readonly)
```

Gets the variant value of the thing on the list.

List("ItemList").entries()[1].value == 5;

