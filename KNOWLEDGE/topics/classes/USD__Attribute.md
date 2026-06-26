---
id: usd__attribute
name: "USD::Attribute"
kind: class
module: "USD"
signature: "USD::Attribute"
aliases: []
tags: ["class", "usd-module"]
deprecated: false
replacedBy: null
description: "A class that represents a USD Property."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\USD.Attribute.xml"
---

# USD::Attribute

A class that represents a USD Property.

## Methods

### clear

```flexscript
USD::Attribute.clear()
```

Removes all time samples and values from the attribute.

### clearAtTime

```flexscript
USD::Attribute.clearAtTime(Variant timeCode)
```

Removes a value with a specific time code from the attribute.

**Parameters:**
- `timeCode` *(Variant)* — The time code of the value to be removed from the attribute.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim operator = stage.findPrim("/World/Operator1");
USD.Attribute translation = prim.getProperty("xformOp:translate");
// if there is at least 1 time sample
if (translation.numTimeSamples) {
Array times = translation.getTimeSamples();
// remove the first time sample
translation.clearAtTime(times[1]);
}
...

### getTimeSamples

```flexscript
Array USD::Attribute.getTimeSamples()
```

Gets an Array of all the time samples for the attribute.

**Returns:** An Array of time samples.

### getValue

```flexscript
Variant USD::Attribute.getValue(Variant timeCode = nullvar)
```

Gets the value of the attribute.

**Parameters:**
- `timeCode` *(Variant, default nullvar)* — The timecode of the value you want to retrieve.

**Returns:** The value of the attribute.

If a timecode is supplied, this will get the value of the attribute
at the given timecode.

### setValue

```flexscript
USD::Attribute.setValue(Variant value, Variant timeCode = nullvar)
```

Sets the value of the attribute.

**Parameters:**
- `value` *(Variant)* — The new value to set the attribute to.
- `timeCode` *(Variant, default nullvar)* — The timecode of the value you want to set.

If a timecode is supplied, this will set the value of the attribute
at the given timecode.

## Properties

### numTimeSamples

```flexscript
int USD::Attribute.numTimeSamples (readonly)
```

Gets the number of time samples the attribute has.

### type

```flexscript
string USD::Attribute.type (readonly)
```

The type of the attribute.

Basic attribute types can be found here.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
USD.Attribute attr = prim.createAttribute("myDouble", "double");
if (attr) {
string type = attr.type; // type == "double"
}

### value

```flexscript
Variant USD::Attribute.value
```

The value of the attribute.

If you set the value of the attribute, the type of value you assign must match the type
of attribute it is. For example, if my attribute has a type of "double[]", then my value should be an Array of doubles.

USD.Stage stage = Model.find("USD Stage1");
USD.Prim prim = stage.defaultPrim;
USD.Attribute attr = prim.createAttribute("myDouble", "double");
if (attr) {
attr.value = 20;
}

