---
id: mat4
name: "Mat4"
kind: class
module: ""
signature: "Mat4"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Represents a matrix for performing space/vector transformations."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Mat4.xml"
---

# Mat4

Represents a matrix for performing space/vector transformations.

## Methods

### inverse

```flexscript
Mat4 Mat4.inverse()
```

Returns a matrix that is the inverse of this matrix.

**Returns:** The inverted matrix.

This method creates new matrix that is the inverse of the matrix and returns it.

### Mat4

```flexscript
Mat4 Mat4.Mat4(Variant values)
```

Constructs a matrix.

**Parameters:**
- `values` *(Variant)* — The array of 16 values to construct the matrix by. The array can either be 4x4 or flat 16.

### rotate

```flexscript
Mat4 Mat4.rotate(double degrees, Vec3 vec)
```

Rotates the transformation matrix around a defined vector.

**Parameters:**
- `degrees` *(double)* — The number of degrees to rotate.
- `vec` *(Vec3)* — The vector around which to rotate.

**Returns:** The owning rotated matrix.

This method rotates the owning matrix and returns a reference to itself, instead of creating a copy
of the matrix.

### scale

```flexscript
Mat4 Mat4.scale(Vec3 vec)
```

Scales the matrix by a defined x/y/z vector.

**Parameters:**
- `vec` *(Vec3)* — The vector to scale by.

**Returns:** The owning scaled matrix.

This method scales the owning matrix and returns a reference to itself, instead of creating a copy
of the matrix.

### translate

```flexscript
Mat4 Mat4.translate(Vec3 vec)
```

Translates the matrix by a defined x/y/z vector.

**Parameters:**
- `vec` *(Vec3)* — The vector to translate by.

**Returns:** The owning translated matrix.

This method translates the owning matrix and returns a reference to itself, instead of creating a copy
of the matrix.

