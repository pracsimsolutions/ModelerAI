---
id: variant
name: "Variant"
kind: class
module: ""
signature: "Variant"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A type that can hold either a number, a string, a treenode or an Array."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Variant.xml"
---

# Variant

A type that can hold either a number, a string, a treenode or an Array.

The Variant class is an essential type by which data is exchanged between various
functionalities in FlexSim. As such, the Variant represents a "jack of all trades" that can
hold several different types of data. It can be a number type, holding a number value. It can
be a string type, holding text data. It can be a treenode
type, holding a reference to a node in FlexSim's tree. It can be an Array type,
holding an array of Variants. It can be a Map type, holding a set of keys and their
associated values. And finally, it can be a null variant meaning it is none of the other types.
Method/Property Forwarding
Since the Variant is just a flexible container for other data types, its methods and properties
are just forwarders to the various methods and properties of the types that the Variant holds. For
example, if you have a Variant holding a treenode, you can use all of the methods
and properties on a treenode directly on the Variant without having to cast to a treenode with the
as() keyword.
Variant node = param(1);
node.name = "Object1";
node.value = 5;
The only property that is exclusive to the Variant is type,
which tells you what type of data is held by a Variant. All other properties and methods simply forward
to the associated method or property of the held class. If the Variant does not hold the associated type, then
calling the type's method will result in an exception. For example, if a Variant has a number, and you call
the find() method on it, FlexScript will throw an exception.

Casting
Variant will cast to any of its target types without throwing an exception. If the Variant does not
hold the target type, it will just give a null-or-similar result. This means a NULL treenode, empty string,
empty Array, or 0.0 number.
Variant num = 1;
string asString = num; // empty string
Variant str = "Hello";
double asNum = str; // 0.0
To determine the type of data that is held by a Variant, use the
type property, or use the is() keyword.

Variants as Objects
If a variant holds a treenode with
Object data, it can access Object properties and methods.
However, the type property is simply a treenode type.
In order to test if a Variant holds an Object node,
you can use the is() keyword:

Variant obj = param(1);
if (obj.is(Object)) {
obj.location = Vec3(1, 2, 3);
}

Variants as Arrays
The Variant implements special logic associated with arrays in that you can treat a Variant like an
Array even if it does not hold array data. If a Variant holds, for example,
a string, you can treat the Variant as if it were a 1-length Array with a
string as its first element.
Variant val = "Hello";
int len = val.length; // len = 1
return val[len]; // returns "Hello";
Also, if you cast a Variant into a non-array type, and the Variant is in fact an Array, the
cast operation will return the first element of the array. Thus you can even treat an Array
Variant as a non-array type.
Variant myArray = ["Hello", "World"];
string myString = myArray;
return myString; // returns "Hello"
These capabilities are especially useful in Process Flow, where you are often dealing with
label values that may or may not contain array data, but you want to interface with those
data in a consistent manner.

Variants as Maps

To use a Variant as a Map, you can use the dynamic properties of the Variant:
// adds the key "Key1" with the value 3 to the map
mapVariant.Key1 = 3
No other methods or properties of the Map are accessible from the Variant. To use
them, you'll need to cast the Variant as a Map:
Map map = mapVariant;
// now you can access map methods and properties
Array keys = map.keys;

Null Values

To check if a Variant is null you can compare the value with the special keyword nullvar or compare
its type with the VAR_TYPE_NULL macro.
Variant val;
val == nullvar //true
val.type == VAR_TYPE_NULL // true

Where this gets more complicated is when dealing with treenode type Variants returned by other properies. See the discussion of treenode null values.

Other data types do not have null types but have an empty type that corresponds to that type:

string emptyString = "";
int emptyInt = 0;
double emptyDouble = 0.0;
Array emptyArray = [];

Awaitable
A Variant is awaitable, meaning it can be the predicate of an
await statement. This will only
cause suspension of the coroutine if the Variant is a reference to a
Coroutine node that has been suspended. You can get a reference to a
coroutine node by calling treenode.
coevaluate() on a node that itself contains an
await statement. Awaiting any other type of Variant value will do nothing.

## Methods

### append

```flexscript
Variant Variant.append(Variant val)
```

Appends another Variant to this Variant.

**Parameters:**
- `val` *(Variant)* — The value to append to this Variant.

**Returns:** The modified Variant

If this Variant is already an Array, this forwards to Array.append().
If the Variant is a null Variant, it will become val. If it is a non-array type, it will convert itself into an Array, and forward to
Array.append().

### byteToCharIndex

```flexscript
int Variant.byteToCharIndex(int byteIndex)
```

Returns the index of a given byte in a string Variant.

**Parameters:**
- `byteIndex` *(int)* — The 1-based byte index of the target character.

**Returns:** The 1-based character index of the character.

Forwards to string.byteToCharIndex.
An exception is thrown if the Variant is not a string type.

### charAt

```flexscript
string Variant.charAt(int byteIndex)
```

Returns the character at a given byte index in a string Variant.

**Parameters:**
- `byteIndex` *(int)* — The 1-based byte index of the target character.

**Returns:** The target character, as a string.

Forwards to string.charAt.
An exception is thrown if the Variant is not a string type.

### charToByteIndex

```flexscript
int Variant.charToByteIndex(int charIndex)
```

Returns the byte index associated with a specific character index in a string Variant.

**Parameters:**
- `charIndex` *(int)* — The 1-based character index of the target character.

**Returns:** The 1-based byte index of the character.

Forwards to string.charToByteIndex.
An exception is thrown if the Variant is not a string type.

### clone

```flexscript
Variant Variant.clone()
```

Clones a Variant.

**Returns:** A clone of the Variant.

If the Variant is an Array type, this forwards to Array.clone().
Otherwise it just returns a copy of its value.

### concat

```flexscript
Array Variant.concat(Array other)
```

Concatenates two Variants into an Array.

**Parameters:**
- `other` *(Array)* — The Array to concatenate onto the end of this Variant.

**Returns:** The resulting Array.

Forwards to Array.concat().

### copy

```flexscript
treenode Variant.copy(treenode destination = 0, int replace = 0)
```

Creates a new copy of a treenode Variant.

**Parameters:**
- `destination` *(treenode, default 0)*
- `replace` *(int, default 0)*

Forwards to treenode.copy().
An exception is thrown if the Variant is not a treenode type.

### destroy

```flexscript
Variant.destroy()
```

Destroys a treenode Variant.

Forwards to treenode.destroy().
An exception is thrown if the Variant is not a treenode type.

### endsWith

```flexscript
int Variant.endsWith(string str)
```

Queries whether a string Variant ends with a defined string.

**Parameters:**
- `str` *(string)*

Forwards to string.endsWith().
An exception is thrown if the Variant is not a string type.

### evaluate

```flexscript
Variant Variant.evaluate()
```

Evaluates a treenode Variant

Forwards to treenode.evaluate().
An exception is thrown if the Variant is not a treenode type.

### fill

```flexscript
Variant Variant.fill(Variant value)
```

Fills an Array Variant with a defined value.

**Parameters:**
- `value` *(Variant)*

Forwards to Array.fill().
If the Variant is not already an Array, it simply sets its value to the passed value.

### find

```flexscript
treenode Variant.find(string path)
```

Finds a node in a treenode Variant's sub-tree by path.

**Parameters:**
- `path` *(string)*

Forwards to treenode.find().
An exception is thrown if the Variant is not a treenode type.

### getLocation

```flexscript
Vec3 Variant.getLocation(double xFactor, double yFactor, double zFactor)
```

Gets an Object Variant's location.

**Parameters:**
- `xFactor` *(double)* — The x value of the point on the object where the location will be measured from. Measured as a percent of the object's size (0-1).
- `yFactor` *(double)* — The y value of the point on the object where the location will be measured from. Measured as a percent of the object's size (0-1).
- `zFactor` *(double)* — The z value of the point on the object where the location will be measured from. Measured as a percent of the object's size (0-1).

**Returns:** A vector holding the object's location

Forwards to Object.getLocation.
This method throws an exception if the Variant is not a treenode with Object data.

### getPath

```flexscript
string Variant.getPath(treenode relativeTo = 0, int byName = 1)
```

Gets the path of a treenode Variant.

**Parameters:**
- `relativeTo` *(treenode, default 0)*
- `byName` *(int, default 1)*

Forwards to treenode.getPath().
An exception is thrown if the Variant is not a treenode type.

### getVariable

```flexscript
Variant Variant.getVariable(string variableName)
```

Gets the value of an Object Variant's variable.

**Parameters:**
- `variableName` *(string)* — The name of the variable

**Returns:** The value of the specified variable.

Forwards to Object.getVariable.
This method throws an exception if the Variant is not a treenode with Object data.

### includes

```flexscript
int Variant.includes(string str)
```

Queries whether a string Variant includes a string.

**Parameters:**
- `str` *(string)*

Forwards to string.includes().
An exception is thrown if the Variant is not a string type.

### indexOf

```flexscript
int Variant.indexOf(Variant )
```

Gets the index of an element in an Array Variant.

**Parameters:**
- `` *(Variant)*

Forwards to Array.indexOf().
If the Variant is not an Array, it is treated as an array of length 1.

### join

```flexscript
string Variant.join(string separator = 0)
```

Joins an Array Variant into a string.

**Parameters:**
- `separator` *(string, default 0)*

Forwards to Array.join().
If the Variant is not already an Array, it will create a new Array from itself, and call join
on that Array.

### match

```flexscript
Array Variant.match(RegExp pattern)
```

Matches a reqular expression on a string Variant.

**Parameters:**
- `pattern` *(RegExp)*

Forwards to string.match().
An exception is thrown if the Variant is not a string type.

### message

```flexscript
Variant Variant.message(double delayTime = 0, treenode fromObj = 0, Variant p1 = nullvar, Variant p2 = nullvar, Variant p3 = nullvar)
```

Send a message to the Object Variant.

**Parameters:**
- `delayTime` *(double, default 0)* — Optional. Specifies the amount of time to delay before firing the message. If -1, the message will be sent synchronously. If 0 or greater, an event will be created in the defined delay time, and the message will be fired after that delay time.
- `fromObj` *(treenode, default 0)* — Optional. The object that is sending the message. This can be accessed from the message trigger.
- `p1` *(Variant, default nullvar)* — Optional. Defines the first parameter of the message. This can be accessed from the message trigger as msgparam1.
- `p2` *(Variant, default nullvar)* — Optional. Defines the second parameter of the message. This can be accessed from the message trigger as msgparam2.
- `p3` *(Variant, default nullvar)* — Optional. Defines the third parameter of the message. This can be accessed from the message trigger as msgparam3.

**Returns:** If the message is sent synchronously (delay = -1), it will return whatever is returned by the object's message trigger. If the message is sent asynchronously (delay >= 0), the call will return a reference to the event node that was created in the event list.

Forwards to Object.message.
This method throws an exception if the Variant is not a treenode with Object data.

### padEnd

```flexscript
string Variant.padEnd(int targetLength, string padString)
```

Pads the end of a string Variant with the padString to the given length.

**Parameters:**
- `targetLength` *(int)* — The desired length of the padded string.
- `padString` *(string)* — The string used to pad the current string.

**Returns:** The string padded to the specified length.

Forwards to string.padEnd.
An exception is thrown if the Variant is not a string type.

### padStart

```flexscript
string Variant.padStart(int targetLength, string padString)
```

Pads the start of a string Variant with the padString to the given length.

**Parameters:**
- `targetLength` *(int)* — The desired length of the padded string.
- `padString` *(string)* — The string used to pad the current string.

**Returns:** The string padded to the specified length.

Forwards to string.padStart.
An exception is thrown if the Variant is not a string type.

### pop

```flexscript
Variant Variant.pop()
```

Pops an element from an Array Variant.

Forwards to Array.pop().
If the Variant is not an Array, it will convert itself into a null Variant, and return its
original value.

### push

```flexscript
Variant Variant.push(Variant value)
```

Pushes a value onto an Array Variant.

**Parameters:**
- `value` *(Variant)*

**Returns:** The modified Variant.

Forwards to Array.push().
If the Variant is a non-array value, it will convert itself into an Array, and forward to
Array.push(). If it is a null Variant,
it will become the passed value (not an Array).

### repeat

```flexscript
string Variant.repeat(int numRepeats)
```

Repeats a string Variant a number of times.

**Parameters:**
- `numRepeats` *(int)*

Forwards to string.repeat().
An exception is thrown if the Variant is not a string type.

### replace

```flexscript
string Variant.replace(string findStr, string replaceWith)
```

Replaces text in a string Variant

**Parameters:**
- `findStr` *(string)*
- `replaceWith` *(string)*

Forwards to string.replace().
An exception is thrown if the Variant is not a string type.

### resume

```flexscript
Variant.resume(int id = 0, int stateProfile = 0)
```

Return the Object Variant to its state prior to stop().

**Parameters:**
- `id` *(int, default 0)* — Optional. For matching stop requests with resume requests.
- `stateProfile` *(int, default 0)* — Optional. The state profile on which to record the state.

Forwards to Object.resume.
This method throws an exception if the Variant is not a treenode with Object data.

### reverse

```flexscript
Variant Variant.reverse()
```

Reverses an Array Variant

Forwards to Array.reverse().
If the Variant is a non-array value, it will convert itself into an Array, and forward to
Array.reverse(). If it is a null Variant,
it will become the passed value (not an Array).

### search

```flexscript
int Variant.search(string str)
```

Searches a string Variant for an internal string or pattern.

**Parameters:**
- `str` *(string)* — The string or pattern to search for.

**Returns:** If found, the 1-based byte index of the found string. Otherwise -1.

Forwards to string.search.
An exception is thrown if the Variant is not a string type.

### setLocation

```flexscript
Object Variant.setLocation(double x, double y, double z)
```

**Parameters:**
- `x` *(double)* — The desired x position.
- `y` *(double)* — The desired y position.
- `z` *(double)* — The desired z position.

**Returns:** The translated object.

Forwards to Object.setLocation.
This method throws an exception if the Variant is not a treenode with Object data.

### setRotation

```flexscript
Object Variant.setRotation(double x, double y, double z)
```

Sets the Object Variant's rotation.

**Parameters:**
- `x` *(double)* — The desired rotation about the x axis.
- `y` *(double)* — The desired rotation about the y axis.
- `z` *(double)* — The desired rotation about the z axis.

**Returns:** The rotated object.

Forwards to Object.setRotation.
This method throws an exception if the Variant is not a treenode with Object data.

### setSize

```flexscript
Object Variant.setSize(double x, double y, double z)
```

Sets the Object Variant's size.

**Parameters:**
- `x` *(double)* — The desired x size.
- `y` *(double)* — The desired y size.
- `z` *(double)* — The desired z size.

**Returns:** The sized object.

Forwards to Object.setSize.
This method throws an exception if the Variant is not a treenode with Object data.

### setState

```flexscript
Variant.setState(int state, int profile)
```

Sets the Object Variant's state.

**Parameters:**
- `state` *(int)* — The desired state.
- `profile` *(int)* — The desired state profile.

Forwards to Object.setState.
This method throws an exception if the Variant is not a treenode with Object data.

### setVariable

```flexscript
Variant.setVariable(string variableName, Variant value)
```

Sets the value of an Object Variant's variable.

**Parameters:**
- `variableName` *(string)* — The name of the variable.
- `value` *(Variant)* — The desired variable value.

Forwards to Object.setVariable.
This method throws an exception if the Variant is not a treenode with Object data.

### shift

```flexscript
Variant Variant.shift()
```

Shifts (pops) an element from the front of an Array Variant.

Forwards to Array.shift().
If the Variant is not an Array, it will convert itself into a null Variant, and return its
original value.

### slice

```flexscript
Array Variant.slice(int fromIndex, int toIndex)
```

Returns a new Array made up of a subsection of an Array Variant.

**Parameters:**
- `fromIndex` *(int)*
- `toIndex` *(int)*

Forwards to Array.slice().
If the Variant is not an Array, it will create an Array from itself and forward to
Array.slice().

### sort

```flexscript
Variant Variant.sort()
```

Sorts an Array Variant

Forwards to Array.sort().
If the Variant is a non-array value, it will convert itself into an Array, and forward to
Array.sort(). If it is a null Variant,
it will become the passed value (not an Array).

### splice

```flexscript
Variant Variant.splice(int fromIndex, int removeCount)
```

Removes and/or inserts elements into an Array Variant.

**Parameters:**
- `fromIndex` *(int)*
- `removeCount` *(int)*

**Returns:** The modified Variant

Forwards to Array.splice().
If the Variant is not an Array, it will make itself into an Array and forward to
Array.splice().

### split

```flexscript
Array Variant.split(string delimiter, int limit = -1)
```

Splits a string Variant into an Array.

**Parameters:**
- `delimiter` *(string)*
- `limit` *(int, default -1)*

Forwards to string.split().
An exception is thrown if the Variant is not a string type.

### startsWith

```flexscript
int Variant.startsWith(string str)
```

Queries if a string Variant starts with a defined string.

**Parameters:**
- `str` *(string)*

Forwards to string.startsWith().
An exception is thrown if the Variant is not a string type.

### stop

```flexscript
Variant.stop(int stopState, int id = 0, double priority = 0, int stateProfile = 0)
```

Stops the Object Variant, sets its state, and waits for resume().

**Parameters:**
- `stopState` *(int)* — The state the object will go into.
- `id` *(int, default 0)* — Optional. For matching stop requests with resume requests.
- `priority` *(double, default 0)* — Optional. For breaking ties between stop() calls.
- `stateProfile` *(int, default 0)* — Optional. The state profile on which to record the state.

Forwards to Object.stop.
This method throws an exception if the Variant is not a treenode with Object data.

### substr

```flexscript
string Variant.substr(int startIndex, int length)
```

Extracts a section of a string Variant and returns a new string.

**Parameters:**
- `startIndex` *(int)* — The 1-based byte index of the first character to extract. If this index is in the middle of a multi-byte character, it will extract the full character. If it is a negative number, the start position is string.length + beginIndex, or in other words it is an offset from the end of the string.
- `length` *(int)* — The length of the target string, in bytes.

**Returns:** The extracted string.

Forwards to string.substr.
An exception is thrown if the Variant is not a string type.

### toLowerCase

```flexscript
string Variant.toLowerCase()
```

Creates a lower case string from a string Variant.

Forwards to string.toLowerCase().
An exception is thrown if the Variant is not a string type.

### toNum

```flexscript
double Variant.toNum()
```

Interprets a string Variant's content as a floating point number.

**Returns:** The interpreted content of the string as a double.

Forwards to string.toNum.
An exception is thrown if the Variant is not a string type.

### toUpperCase

```flexscript
string Variant.toUpperCase()
```

Creates an upper case string from a string Variant.

Forwards to string.toUpperCase().
An exception is thrown if the Variant is not a string type.

### trim

```flexscript
string Variant.trim()
```

Trims white space off of the ends of a string Variant.

Forwards to string.trim().
An exception is thrown if the Variant is not a string type.

### unshift

```flexscript
Variant Variant.unshift(Variant value)
```

Unshifts (pushes) a value onto the front of an Array Variant.

**Parameters:**
- `value` *(Variant)*

**Returns:** The modified Variant.

Forwards to Array.unshift().
If the Variant is a non-array value, it will convert itself into an Array, and forward to
Array.unshift(). If it is a null Variant,
it will become the passed value (not an Array).

### Variant

```flexscript
Variant.Variant(int value)
```

Construct a Variant from a value.

**Parameters:**
- `value` *(int)*

## Properties

### animations

```flexscript
Variant.animations (readonly)
```

Returns the animations property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### attrs

```flexscript
Variant.attrs (readonly)
```

Returns the attrs property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### centerObjects

```flexscript
Variant.centerObjects (readonly)
```

Returns the centerObjects property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### color

```flexscript
Color Variant.color (readonly)
```

Returns the color property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### dataType

```flexscript
int Variant.dataType
```

Gets/sets the dataType property on a treenode Variant.

An exception is thrown if the Variant is not a treenode type.

### first

```flexscript
treenode Variant.first
```

Gets the first subnode of a treenode Variant.

Forwards to treenode.first.
An exception is thrown if the Variant is not a treenode type.

### flags

```flexscript
Variant.flags (readonly)
```

Returns the flags property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### inObjects

```flexscript
Variant.inObjects (readonly)
```

Returns the inObjects property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### input

```flexscript
Variant.input (readonly)
```

Returns the input property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### labels

```flexscript
Variant.labels (readonly)
```

Accesses the labels of a treenode Variant, or the string keys of a Map variant.

Forwards to treenode.labels,
or to the Map.stringKeys.
An exception is thrown if the Variant is not a treenode or Map type.

### last

```flexscript
treenode Variant.last
```

Gets the last subnode of a treenode Variant.

Forwards to treenode.last.
An exception is thrown if the Variant is not a treenode type.

### length

```flexscript
int Variant.length
```

Gets/sets the Array length of a Variant.

If the Variant is already an Array type, this will forward to
Array.length. If the Variant
is a null Variant, length will return 0. If it is a number, string,
or treenode type, length will return 1.
Variant myVar; // initialized as null Variant
myVar.length; // returns 0
myVar = 5; // set to number type
myVar.length; // returns 1
If length is set, the Variant will convert itself into an Array, and forward to
Array.length
Variant myArray = "Hello";
myArray.length = 2; // converts to Array and sets Array length to 2.

### location

```flexscript
Vec3 Variant.location (readonly)
```

Returns the location property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### name

```flexscript
string Variant.name
```

Gets/sets the name of a treenode Variant.

Forwards to treenode.name.
An exception is thrown if the Variant is not a treenode type.

### next

```flexscript
treenode Variant.next (readonly)
```

Gets the next node of a treenode Variant.

Forwards to treenode.next.
An exception is thrown if the Variant is not a treenode type.

### outObjects

```flexscript
Variant.outObjects (readonly)
```

Returns the outObjects property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### output

```flexscript
Variant.output (readonly)
```

Returns the output property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### prev

```flexscript
treenode Variant.prev (readonly)
```

Gets the next node of a treenode Variant.

Forwards to treenode.prev.
An exception is thrown if the Variant is not a treenode type.

### rank

```flexscript
int Variant.rank
```

Gets/sets the rank of a treenode Variant.

Forwards to treenode.rank.
An exception is thrown if the Variant is not a treenode type.

### resetPosition

```flexscript
Variant.resetPosition (readonly)
```

Returns the resetPosition property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### rotation

```flexscript
Vec3 Variant.rotation (readonly)
```

Returns the rotation property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### size

```flexscript
Vec3 Variant.size (readonly)
```

Returns the size property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### stats

```flexscript
Variant.stats (readonly)
```

Returns the stats property of an Object Variant.

This property throws an exception if the Variant is not a treenode with Object data.

### subnodes

```flexscript
Variant.subnodes (readonly)
```

Accesses the subnodes of a treenode Variant.

Forwards to treenode.subnodes.
An exception is thrown if the Variant is not a treenode type.

### type

```flexscript
int Variant.type (readonly)
```

Gets a Variant's type, one of VAR_TYPE_NUMBER, VAR_TYPE_STRING, VAR_TYPE_NODE, or VAR_TYPE_ARRAY

### up

```flexscript
treenode Variant.up
```

Gets/sets the parent of a treenode Variant.

Forwards to treenode.up.
An exception is thrown if the Variant is not a treenode type.

### value

```flexscript
Variant Variant.value
```

Gets/sets the data value of a treenode Variant.

Forwards to treenode.value.
An exception is thrown if the Variant is not a treenode type.

