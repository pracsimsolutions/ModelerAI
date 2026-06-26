---
id: vec3
name: "Vec3"
kind: class
module: ""
signature: "Vec3"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "Represents positions, rotations, size, 3D vectors, and points. Contains functions for doing common vector operations."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Vec3.xml"
---

# Vec3

Represents positions, rotations, size, 3D vectors, and points. Contains functions for doing common vector operations.

## Methods

### angle

```flexscript
double Vec3.angle(Vec3 otherVec)
```

Finds the angle between this vector and another (degrees).

**Parameters:**
- `otherVec` *(Vec3)* — The angle extends round to this vector.

**Returns:** The angle between the two vectors in degrees.

double angle = myVector.angle(Vec3(1, 2, 3));

### cross

```flexscript
Vec3 Vec3.cross(Vec3 otherVec)
```

Finds the cross product of this vector and another.

**Parameters:**
- `otherVec` *(Vec3)* — The vector to cross with this vector.

**Returns:** The cross product (Vec3) of the two vectors.

Vec3 cross = myVector.cross(Vec3(1, 2, 3));
The cross product of two vectors is a vector perpendicular to the two input vectors. Since there are two possible perpendicular vectors, the direction is determined by the "left hand rule". This means that crossing vector1 with vector2 will
produce the opposite of crossing vector2 with vector1. The magnitude of the new vector is equal to the magnitudes of the two inputs multiplied together and then multiplied by the sine of the angle between the inputs.

### dot

```flexscript
double Vec3.dot(Vec3 otherVec)
```

Finds the dot product of this vector and another.

**Parameters:**
- `otherVec` *(Vec3)* — The vector to dot with this vector.

**Returns:** The dot product (double) of the two vectors.

double dot = myVector.dot(Vec3(1, 2, 3));
The dot product of two vectors is a number equal to the magnitudes of the two vectors multiplied together and then multiplied by the cosine of the angle between them.

### lerp

```flexscript
Vec3 Vec3.lerp(Vec3 otherVec, double t)
```

Linearly interpolates between two vectors.

**Parameters:**
- `otherVec` *(Vec3)* — The vector to interpolate to.
- `t` *(double)* — A ratio between 0 and 1 that defines the percentage to interpolate from the current vector to otherVec.

**Returns:** The resulting interpolated vector.

The following example code changes current's location 20% towards the other object.
current.location = current.location.lerp(otherObj.location, 0.2);

### project

```flexscript
Vec3 Vec3.project(treenode from, treenode to)
```

Transforms the vector from one coordinate space to another.

**Parameters:**
- `from` *(treenode)* — The treenode representing the coordinate space to project from.
- `to` *(treenode)* — The treenode representing the coordinate space to project to.

**Returns:** The projected vector.

Takes the vector in from's coordinate space, translates this vector onto to's coordinate space, and returns the projected vector.

The vector is taken from the origin point of from's coordinate space. The origin is the (0,0,0) point of the coordinate space and is often located in the top left corner of an object's coordinate space.
It is important to understand the way the axes work in FlexSim. The X and Y axes form the floor grid plane while the Z axis is equivalent to the height. Also objects y size values stretch in the
-y direction, but stretch in the positive direction on both the X and Z axes. In other words an object at location (0,5,0) with a size of (1,2,3) will stretch from x location 0 to 1, y location 5 to 3 and z location 0 to 3.
Vec3 globalLoc = Vec3(5, 3, 0).project(current, model());
This example finds out the global location of the position (5,3,0) in current's coordinate space.

### projectRotation

```flexscript
Vec3 Vec3.projectRotation(treenode from, treenode to)
```

Transforms the vector, interpreted as a rotation, from one coordinate space to another.

**Parameters:**
- `from` *(treenode)* — The treenode representing the coordinate space to project from.
- `to` *(treenode)* — The treenode representing the coordinate space to project to.

**Returns:** The projected rotation.

Takes the vector, interpreted as a rotation in from's coordinate space, translates this vector onto to's coordinate space, and returns the projected rotation.
Vec3 globalRot = current.rotation.project(current.up, model());
This example finds out the rotation of current, projected onto the model's space.

### Vec3

```flexscript
Vec3.Vec3(double x, double y, double z)
```

Constructs a vector by xyz component values.

**Parameters:**
- `x` *(double)* — The x component.
- `y` *(double)* — The y component.
- `z` *(double)* — The z component.

current.location = Vec3(1, 2, 3);
current.location = Vec3(0);

## Properties

### magnitude

```flexscript
double Vec3.magnitude (readonly)
```

The length of this vector (Read Only).

The length of the vector is calculated as the square root of (x*x+y*y+z*z).

### normalized

```flexscript
Vec3 Vec3.normalized (readonly)
```

The vector sized to a magnitude of 1 (Read Only).

A copy of the original vector that points in the same direction, but has a length of 1.

### x

```flexscript
double Vec3.x
```

X component of the vector.

### y

```flexscript
double Vec3.y
```

Y component of the vector.

### z

```flexscript
double Vec3.z
```

Z component of the vector.

