---
id: usd__reference
name: "USD::Reference"
kind: class
module: "USD"
signature: "USD::Reference"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents an SdfReference from the OpenUSD API."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.Reference.xml"
---

# USD::Reference

A class that represents an SdfReference from the OpenUSD API.

## Methods

### USD.Reference

```flexscript
USD.Reference USD::Reference.USD.Reference(string primPath)
```

Constructs a USD.Reference object based on the given primPath.

**Parameters:**
- `primPath` *(string)* — The prim path of the prim you want to reference.

**Returns:** A USD.Reference object.

## Properties

### assetPath

```flexscript
USD::Reference.assetPath
```

The asset path of the USD.Reference.

### customData

```flexscript
Map USD::Reference.customData
```

A Map of custom data for this USD.Reference.

### isInternal

```flexscript
int USD::Reference.isInternal (readonly)
```

Returns whether or not this USD.Reference is internal.

### layerOffset

```flexscript
double USD::Reference.layerOffset
```

The layer offset of the USD.Reference.

This property can be used to get and set the layer offset property for references to be used
in animations. On USD.Stages, these are set automatically when you export using the Omniverse Connector or save
a USD Stage. However, you can toggle it off on a USD Stage and set them yourself (to 0, for example). This will
allow you to more easily write your own animation information without having to fight with the automatic writing of offsets.

### primPath

```flexscript
USD::Reference.primPath
```

The prim path of the USD.Reference.

