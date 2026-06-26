---
id: usd__prim
name: "USD::Prim"
kind: class
module: "USD"
signature: "USD::Prim"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents a USD Prim."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.Prim.xml"
---

# USD::Prim

A class that represents a USD Prim.

## Methods

### createAttribute

```flexscript
USD.Attribute USD::Prim.createAttribute(string attributeName, string typeName)
```

Create a USD.Attribute on the prim.

**Parameters:**
- `attributeName` *(string)* — The name of the new attribute.
- `typeName` *(string)* — The type of attribute being created.

**Returns:** The newly created USD.Attribute.

The typeName specifices what type of attribute you want to create. For example, you can create a "string" attribute
by running the code below. Basic attribute types can be found here.
If the typeName is invalid, the created USD.Attribute will be invalid.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.definePrim("/my/test/prim", "Xform");
USD.Attribute attribute = prim.createAttribute("testAttr", "string");
if (attribute) {
...
}

### createRelationship

```flexscript
USD.Relationship USD::Prim.createRelationship(relationshipName)
```

Create a USD.Relationship on the prim.

**Parameters:**
- `relationshipName` *(any)*

**Returns:** The newly created USD.Relationship.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.definePrim("/my/test/prim", "Xform");
USD.Relationship relationship = prim.createRelationship("myRelationship");
if (relationship) {
...
}

### getProperty

```flexscript
USD.Property USD::Prim.getProperty(string propertyName)
```

Get a USD.Property with a given name.

**Parameters:**
- `propertyName` *(string)* — The name of the property to search for.

**Returns:** The newly created USD.Property.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.findPrim("/World/Processor1");
USD.Property property = prim.getProperty("myProperty");
if (property) {
...
}

### getPropertyNames

```flexscript
Array USD::Prim.getPropertyNames()
```

Get an Array of names of properties on the prim.

**Returns:** An array of USD.Prim objects.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
Array propertyNames = prim.getPropertyNames();
for (int i = 1; i <= propertyNames.length; i++) {
...
}

## Properties

### path

```flexscript
string USD::Prim.path (readonly)
```

The complete path for the prim.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
string path = prim.path;

### references

```flexscript
USD::Prim.references (readonly)
```

Allows a user to clear and set references on a USD.Prim. See USD.References for more info.

// Clear the current offsets from the operator's refshape and reset them to 0
USD.Stage stage = Model.find("USD Stage1");
USD.Prim refShape = stage.findPrim("/World/Operator1/shapeOffset/refShape");
USD.Reference ref = USD.Reference("/shapes/tn__OperatorMalefbx_ZN");
ref.layerOffset = 0;
refShape.references.clear();
refShape.references.add(ref);
stage.save();

