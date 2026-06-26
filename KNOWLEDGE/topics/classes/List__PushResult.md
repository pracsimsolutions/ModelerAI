---
id: list__pushresult
name: "List::PushResult"
kind: class
module: "List"
signature: "List::PushResult"
aliases: []
tags: ["class", "list-module"]
deprecated: false
replacedBy: null
description: "A class that represents the result of a call to List.push()."
seeAlso: []
source: "help\\FlexScriptAPIReference\\List\\List.PushResult.xml"
---

# List::PushResult

A class that represents the result of a call to List.push().

The List.PushResult can be treated just like a Variant.

## Properties

### entries

```flexscript
List::PushResult.entries (readonly)
```

The resulting entries, if any exist.

If the pushed object is not immediately pulled, an entry is created. This
property gives access to that entry if one exists.
List.PushResult result = List("ItemList").push(item);
result.entries.length // Number of entries just pushed
result.entries[1] // A reference to the first entry just pushed on the list

