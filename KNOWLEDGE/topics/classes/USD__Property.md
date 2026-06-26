---
id: usd__property
name: "USD::Property"
kind: class
module: "USD"
signature: "USD::Property"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents a USD Property."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.Property.xml"
---

# USD::Property

A class that represents a USD Property.

## Properties

### baseName

```flexscript
string USD::Property.baseName (readonly)
```

The base name of the property.

The name of the property with all namespace prefixes removed, i.e. the last component of the "name" property.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
USD.Property property = prim.getProperty("my:advanced:prop");
if (property) {
string baseName = property.baseName; // baseName == "prop"
}

### name

```flexscript
string USD::Property.name (readonly)
```

The name of the property.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
USD.Property property = prim.getProperty("my:advanced:prop");
if (property) {
string fullName = property.name; // fullName == "my:advanced:prop"
}

