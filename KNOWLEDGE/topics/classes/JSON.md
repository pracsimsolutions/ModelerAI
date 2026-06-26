---
id: json
name: "JSON"
kind: class
module: ""
signature: "JSON"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class containing common JSON functions."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\JSON.xml"
---

# JSON

A class containing common JSON functions.

## Methods

### parse

```flexscript
Variant (static) JSON.parse(string json)
```

Parses a JSON string and returns a Variant.

**Parameters:**
- `json` *(string)* — The JSON string.

**Returns:** The parsed JSON as a Variant.

Array array = JSON.parse("[0, \"string\"]");
Map map = JSON.parse("{\"number\" : 0, \"string\" : \"string\"}");

### stringify

```flexscript
string (static) JSON.stringify(Variant value)
```

Converts a Variant into a JSON string.

**Parameters:**
- `value` *(Variant)* — The Variant value.

**Returns:** The resulting JSON string.

string str1 = JSON.stringify([0, "string"]);
Map map;
map["number"] = 0;
map["string"] = "string";
string str2 = JSON.stringify(map);

