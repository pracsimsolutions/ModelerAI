---
id: array
name: "Array"
kind: class
module: ""
signature: "Array"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A dynamic array of Variants."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Array.xml"
---

# Array

A dynamic array of Variants.

Initialize an Array of 5 elements:Array myArray = Array(5);
Arrays in FlexSim are 1-based, meaning the first element in the array is accessed via index
1.
double myVal = myArray[1]; // 1st element
myArray[2] = 5; // 2nd element
myArray[4] = 3.2; // 4th element
Since the Array is an array of Variants, you can set each element
to anything that a Variant can hold, namely a number, string, treenode,
or Array.
myArray[2] = "Hello World";
myArray[3] = 5.7;
myArray[4] = Model.find("Source1");
myArray[5] = Array(10); // now you can access myArray[5][2]
You can also fill an array with an initializer list.
Array myArray = [4.5, "Hello World", model(), ["Another", "Array"]];
By default, Arrays are passed around as shared references. This means if you set
one Array variable to another Array variable, you are setting the variable to a
reference to the first Array, not a copy of the first Array. Thus, if you change
either variable's referenced Array data, and it will change it for both variables.
Array array1 = ["Hello", "World"];
Array array2 = array1;
array2[1] = "Goodbye"; // both array1 and array2 are ["Goodbye", "World"]
To do a deep copy of an Array, use the clone() method.
Array array1 = ["Hello", "World"];
Array array2 = array1.clone();
array2[1] = "Goodbye"; // array2 is ["Goodbye", "World"], array1 is unchanged.

## Methods

### append

```flexscript
Array Array.append(Array otherArray)
```

Appends another array to the end of the array.

**Parameters:**
- `otherArray` *(Array)* — The array to append to this array.

**Returns:** The modified array.

Array myArray = [1,2,3,4,5];
myArray.append([7,8,9]); //[1,2,3,4,5,7,8,9]

### Array

```flexscript
Array.Array(int size)
```

Creates a new array of the given size.

**Parameters:**
- `size` *(int)* — The size of the new array.

Array myArray = Array(5); // [,,,,]

### clone

```flexscript
Array Array.clone()
```

Creates a copy of the array.

**Returns:** A copy of the array.

Array myArray = [1,2,3,4,5];
Array copy = myArray.clone();
myArray.push(6);

// myArray - [1,2,3,4,5,6]
// copy - [1,2,3,4,5]

### concat

```flexscript
Array Array.concat(Array otherArray)
```

Creates a new array by appending another array to the end of the array.

**Parameters:**
- `otherArray` *(Array)* — The array to concatenate with this array.

**Returns:** A new array made by concatenating the arrays.

Array concatArray = myArray.concat([7,8,9]);

// myArray - [1,2,3,4,5]
// concatArray - [1,2,3,4,5,7,8,9]

Similar to the append method except that the append method modifies the original array, whereas concat makes a
new array.

### fill

```flexscript
Array Array.fill(Variant val)
```

Fills the array with a value.

**Parameters:**
- `val` *(Variant)* — The value each element will now hold.

**Returns:** The modified array.

This code sets each element to 0.

Array myArray = [1,2,3,4,5];
myArray.fill(0); // [0,0,0,0,0]

### indexOf

```flexscript
int Array.indexOf(Variant val)
```

Searches the array for an element and returns its index.

**Parameters:**
- `val` *(Variant)* — The element to look for.

**Returns:** The index of the first occurence of the element in the array.

Returns -1 if the element is not found in the array.

Array myArray = ["Apple","Orange","Mango","Pear"];
int index = myArray.indexOf("Mango"); // 3
int index = myArray.indexOf(2); // -1

### join

```flexscript
string Array.join(string separator = 0)
```

Makes a string by concatenating each element.

**Parameters:**
- `separator` *(string, default 0)* — A string value that will be placed between elements when concatenating.

**Returns:** The string of concatenated elements separated by the separator.

Array myArray = [1,2,3,4,5];
string joined = myArray.join("-"); // "1-2-3-4-5"

### pop

```flexscript
Variant Array.pop()
```

Removes and returns the last element of an array.

**Returns:** The last element of the array.

Array myArray = [1,2,3,4,5];
Variant lastVal = myArray.pop(); // 5

//myArray - [1,2,3,4]

### push

```flexscript
Array Array.push(Variant val)
```

Adds a new element to the end of the array.

**Parameters:**
- `val` *(Variant)* — The new array element.

**Returns:** The modified array.

Array myArray = [1,2,3,4,5];
myArray.push(6); // [1,2,3,4,5,6]

### reverse

```flexscript
Array Array.reverse()
```

Reverses the order of the elements in the array.

**Returns:** The modified array.

Array myArray = [1,2,3,4,5];
myArray.reverse(); // [5,4,3,2,1]

### shift

```flexscript
Variant Array.shift()
```

Removes and returns the first element of an array.

**Returns:** The first element of the array.

Array myArray = [1,2,3,4,5];
Variant firstVal = myArray.shift(); // 1

//myArray - [2,3,4,5]

### slice

```flexscript
Array Array.slice(int fromIndex, int toIndex)
```

Copies part of the array.

**Parameters:**
- `fromIndex` *(int)* — The index of the first element in the selection.
- `toIndex` *(int)* — The index of the element after the last element in the selection.

**Returns:** A copy of part of the array.

Array myArray = [1,2,3,4,5];
Array copy = myArray.slice(2, 5); // [2,3,4]

//myArray - [1,2,3,4,5]

Selects the elements starting at fromIndex, and ends at, but does not include, toIndex.

### sort

```flexscript
Array Array.sort()
```

Sorts the elements in the array.

**Returns:** The modified array.

Array myArray = [1,3,5,2,4];
myArray.sort(); // [1,2,3,4,5]

Array myArray2 = ["a","c","e","b","d"];
myArray2.sort(); // ["a","b","c","d","e"]

### splice

```flexscript
Array Array.splice(int fromIndex, int count)
```

Adds and/or removes elements from an array.

**Parameters:**
- `fromIndex` *(int)* — The index where elements will be removed and then added.
- `count` *(int)* — The number of elements to remove.

**Returns:** The modified array.

Array cars = ["Volvo", "BMW", "Audi", "Ford"];
cars.splice(2, 2); // ["Volvo", "Ford"]
cars.splice(3, 1, ["Lotus", "Kia"]); // ["Volvo", "BMW", "Lotus", "Kia", "Ford"]

### unshift

```flexscript
Array Array.unshift(Variant val)
```

Adds a new element to the start of the array.

**Parameters:**
- `val` *(Variant)* — The new array element.

**Returns:** The modified array.

Array myArray = [1,2,3,4,5];
myArray.unshift(6); // [6,1,2,3,4,5]

## Properties

### length

```flexscript
int Array.length
```

The number of elements in the array.

int numElements = myArray.length;
Can also be used to set the size of the array.

Array myArray = [1,2,3,4,5];
myArray.length = 3; // [1,2,3]

