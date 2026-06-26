---
id: usd__references
name: "USD::References"
kind: class
module: "USD"
signature: "USD::References"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents a UsdReferences object from the OpenUSD API."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.References.xml"
---

# USD::References

A class that represents a UsdReferences object from the OpenUSD API.

## Methods

### add

```flexscript
int USD::References.add(USD.Reference reference)
```

Adds a reference to the list of references.

**Parameters:**
- `reference` *(USD.Reference)* — The USD.Reference to add to the list.

**Returns:** 1 if the method was successful; otherwise, 0.

### clear

```flexscript
int USD::References.clear()
```

Removes all references from the list of references.

**Returns:** 1 if the method was successful; otherwise, 0.

### remove

```flexscript
int USD::References.remove(string primPath, string assetPath = "")
```

Removes a specific reference from the list of references.

**Parameters:**
- `primPath` *(string)* — The prim path of the reference to remove.
- `assetPath` *(string, default "")* — The asset path of the reference to remove.

**Returns:** 1 if the method was successful; otherwise, 0.

### set

```flexscript
int USD::References.set(Array primPaths)
```

Sets the current list of references to the given list of assets.

**Parameters:**
- `primPaths` *(Array)* — An Array of prim paths as strings.

**Returns:** 1 if the method was successful; otherwise, 0.

Given an Array of strings of prim paths, it will clear the current list of references,
create a USD.Reference for each of the prim paths, and then set the list to the list of USD.References created.
If the Array contains anything that isn't a string, it will throw an exception.

