---
id: map
name: "Map"
kind: class
module: ""
signature: "Map"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "An associative array of Variant keys and values."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Map.xml"
---

# Map

An associative array of Variant keys and values.

A Map contains a set of keys. Each key has an associated value. You can
use the key to retrieve the value. For example:Map map;
map[5] = 10;
int x = map[5]; // x is 10 This example demonstrates that the Map uses the [] operator,
like the Array. However the Map's [] operator has two important differences:
A Map can use any value as a key:Map map;
map["Hello"] = "world";
map[Model.find("Processor1")] = 10;The Map's [] will always add the value in the [] to the Map.
Map map;
map[1];
map[2];
map[3];
// The map now has three keys in it.
// The default value associated with
// each key is a null variant.
A Variant can hold a Map value. For more information, please see the Variant topic.
Dynamic Properties
A Map can use dynamic properties to interact with string keys:
Map map;
map.Key1 = 1; // identical to map["Key1"] = 1;
map.MyKeyName = 3; // identical to map["MyKeyName"] = 3;
return map.Key1; // NOT identical to map["Key1"]
When you use dynamic properties to write to the map, the property is asserted. This is the same behavior
as using the [] operator. However, when you read the map with dynamic properties, you will get an exception
if that key is not present.

Examining a Map
In order to determine if a key is in the map, you can use the find() method:
var iter = map.find(3);
if (iter) {
// the key is contained in the map
print(iter.key, iter.value);
} else {
// the key is not contained in the map
print("key not found");
}
In order to examine each element of a map, you can use the begin() and
end() methods in a for loop:
for (var iter = map.begin(); iter != map.end(); iter++) {
print(iter.key, iter.value);
}
As an alternative, you could use the keys property to access all the keys
in the array, and use a for loop with that array:
Array keys = map.keys;
for (int i = 1; i <= keys.length; i++) {
print(keys[i], map[keys[i]]);
}
Note that the keys property constructs a new array, with a copy of all the keys in the map.

begin(), end(), and find()
The methods begin(), end(), and
find() all return a map iterator object. This object has the following
properties and operators:

key - the key referenced by this iterator.
value - the value associated with the key referenced by this iterator.
operator bool() - casting the iterator as a bool will return true
if the iterator is valid, and false if it is not.
operator++ - incrementing the iterator will move it to the next key
in the map. If the iterator is at the end of the map, this operator invalidates the iterator.

Nodes as Keys
You can use a treenode as a key. You can also use a container (a Map or an Array)
that contains a treenode as a key. However, if that treenode (e.g. a flowitem) is destroyed, then that
key is said to be lost. The keys array will not include those keys, and
an iterator objects (returned from begin(), end(), and
find()) will skip those values.
Key Ordering
The Map class is a hash map, so keys in the map are in not guaranteed to be in any order. They are not sorted by comparison, and they
are not in insertion order. Even if it appears that you can depend on some particular ordering, that ordering could easily change
in a bug fix release or new version of FlexSim.

## Methods

### begin

```flexscript
Map.begin()
```

Returns an iterator to the first key/value pair in the map.

**Returns:** An iterator to the first key/value pair in the map

If the map is empty, then this method returns an iterator equal to the
end()

### clear

```flexscript
Map.clear()
```

Removes all keys and associated values from the Map.

### clone

```flexscript
Map Map.clone()
```

Returns a deep copy of the Map.

**Returns:** A copy of the map

Map map1;
Map map2 = map1; // map2 points at the same map as map1
map1[10] = 2; // map2 can access the key 10
Map map3 = map1.clone();
map3[20]; // does not affect map1 or map2

### end

```flexscript
Map.end()
```

Returns an invalid iterator.

**Returns:** An invalid iterator

This method is usually used as part of a for-loop:
for (var iter = map.begin(); iter != map.end(); iter++) { /*... */}
When the iterator used in the loop advances past the end of the map, it becomes
invalid, and comparing it to the end() iterator will return true.

### find

```flexscript
Map.find(Variant key)
```

Returns an iterator to the given key if it is present, or an invalid iterator if the key is not present.

**Parameters:**
- `key` *(Variant)* — The key to check for in the map

**Returns:** An iterator, invalid if the key is not in the map.

This method does not add the given key to the map, and so
can be used to check for a key without adding it.

### remove

```flexscript
int Map.remove(Variant key)
```

Removes the given key and its associated value from the map if it is present.

**Parameters:**
- `key` *(Variant)* — The key to remove

**Returns:** True if the key was removed, false if the key was not present.

## Properties

### keys

```flexscript
Array Map.keys (readonly)
```

Returns an array of all key values in the map.

The array is a new copy of all keys values. If
keys and values
are both accessed without any intervening insertions or removals, the resulting arrays
will be ordered the same way.

### stringKeys

```flexscript
Map.stringKeys (readonly)
```

Dynamically gets and sets the value for string keys.

Allows getting and setting the value for string keys, similar to
using labels:
// asserts the key "MyKey" and sets its value to 5
map.MyKey = 5;

// reads the value for the key "SKU_1"
// throws if "SKU_1" is not a key in the map
Variant value = map.SKU_1;

### values

```flexscript
Array Map.values (readonly)
```

Returns an aray of all associated values in the map.

he array is a new copy of all associated values. If
keys and values
are both accessed without any intervening insertions or removals, the resulting arrays
will be ordered the same way.

