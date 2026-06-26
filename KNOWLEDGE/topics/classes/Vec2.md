---
id: vec2
name: "Vec2"
kind: class
module: ""
signature: "Vec2"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Represents positions, rotations, size, 2D vectors, and points. Contains functions for doing common vector operations."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Vec2.xml"
---

# Vec2

Represents positions, rotations, size, 2D vectors, and points. Contains functions for doing common vector operations.

## Methods

### angle

```flexscript
double Vec2.angle(Vec2 otherVec)
```

Finds the angle between this vector and another (degrees).

**Parameters:**
- `otherVec` *(Vec2)* — The angle extends round to this vector.

**Returns:** The angle between the two vectors in degrees.

double angle = myVector.angle(Vec2(1, 2));

### dot

```flexscript
double Vec2.dot(Vec2 otherVec)
```

Finds the dot product of this vector and another.

**Parameters:**
- `otherVec` *(Vec2)* — The vector to dot with this vector.

**Returns:** The dot product (double) of the two vectors.

double dot = myVector.dot(Vec2(1, 2));
The dot product of two vectors is a number equal to the magnitudes of the two vectors multiplied together and then multiplied by the cosine of the angle between them.

### lerp

```flexscript
Vec2 Vec2.lerp(Vec2 otherVec, double t)
```

Linearly interpolates between two vectors.

**Parameters:**
- `otherVec` *(Vec2)* — The vector to interpolate to.
- `t` *(double)* — A ratio between 0 and 1 that defines the percentage to interpolate from the current vector to otherVec.

**Returns:** The resulting interpolated vector.

The following example code sets the location vector to 20% on the path from myVector to Vec2(1,2).
Vec2 location = myVector.lerp(Vec2(1, 2), .2);

### rotate

```flexscript
Vec2 Vec2.rotate(double degrees)
```

Rotates the vector about another vector.

**Parameters:**
- `degrees` *(double)* — The angle of rotation.

**Returns:** The resulting vector.

The origin will be used as the rotation point if no other vector is defined
The following example code returns the new vector after rotating myVector about Vec2(1,2) by 45 degrees.
Vec2 location = myVector.rotate(45, Vec2(1, 2));

### Vec2

```flexscript
Vec2.Vec2(double x, double y)
```

Constructs a vector by xy component values.

**Parameters:**
- `x` *(double)* — The x component.
- `y` *(double)* — The y component.

Vec2 location = Vec2(1, 2);
Vec2 location = Vec2(0);

## Properties

### magnitude

```flexscript
double Vec2.magnitude (readonly)
```

The length of this vector (Read Only).

The length of the vector is calculated as the square root of (x*x+y*y).

### normalized

```flexscript
Vec2 Vec2.normalized (readonly)
```

The vector sized to a magnitude of 1 (Read Only).

A copy of the original vector that points in the same direction, but has a length of 1.

### x

```flexscript
double Vec2.x
```

X component of the vector.

### y

```flexscript
double Vec2.y
```

Y component of the vector.

