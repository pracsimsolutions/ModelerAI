---
id: list__pullresult
name: "List::PullResult"
kind: class
module: "List"
signature: "List::PullResult"
aliases: []
tags: ["class", "list-module"]
deprecated: false
replacedBy: null
description: "A class that represents the result of a call to List.pull()."
seeAlso: []
source: "help\\FlexScriptAPIReference\\List\\List.PullResult.xml"
---

# List::PullResult

A class that represents the result of a call to List.pull().

The List.PullResult can be treated just like a Variant. The Variant holds the
value or values that were pulled from the list.
There are several options for writing code to get the result from a list pull.
//define full List.PullResult type
List.PullResult pulled = List("ItemList").pull("WHERE Type == 10", 1, 1);
// just infer the type with var
var pulled = List("ItemList").pull("WHERE Type == 10", 1, 1);
// don't need full List.PullResult properties, so just get it as a Variant
Variant pulled = List("ItemList").pull("WHERE Type == 10", 1, 1);

## Properties

### backOrder

```flexscript
List.BackOrder List::PullResult.backOrder (readonly)
```

The resulting back order, if one exists.

When the pull is not completely fulfilled, a back order associated with the pull is created. This
property gives access to that back order if one exists.
List.PullResult result = List("ItemList").pull("WHERE Type == 10", 1, 1);
if (result.backOrder) {
... // the pull wasn't completely fulfilled, so there is a back order.
}

