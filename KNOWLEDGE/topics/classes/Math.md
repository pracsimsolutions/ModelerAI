---
id: math
name: "Math"
kind: class
module: ""
signature: "Math"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class containing common math functions."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Math.xml"
---

# Math

A class containing common math functions.

## Methods

### acos

```flexscript
double (static) Math.acos(double ratio)
```

Returns the arc cosine of an angle in radians.

**Parameters:**
- `ratio` *(double)* — The ratio - adjacent/hypotenuse - of a right triangle.

**Returns:** The arc cosine of the angle in radians.

acos() is the inverse of the cos() method. Ratio should never
be less than -1 or greater than 1.
double angle = Math.degrees(Math.acos(xdist / totaldist));

### asin

```flexscript
double (static) Math.asin(double ratio)
```

Returns the arc sine of an angle in radians.

**Parameters:**
- `ratio` *(double)* — The ratio - opposite/hypotenuse - of a right triangle.

**Returns:** The arc sine of the angle in radians.

asin() is the inverse of the sin() method. Ratio should never
be less than -1 or greater than 1.
double angle = Math.degrees(Math.asin(ydist / totaldist));

### atan

```flexscript
double (static) Math.atan(double ratio)
```

Returns the arc tangent of an angle in radians.

**Parameters:**
- `ratio` *(double)* — The ratio - opposite/adjacent - of a right triangle.

**Returns:** The arc tangent of the angle in radians.

atan() is the inverse of the tan() method. Ratio should
be within the interval [-pi/2,+
pi/2]. Notice that because of the sign ambiguity, the atan()
method cannot determine with certainty in which quadrant the angle falls only by its tangent value.
You can use the atan2() method if you need to determine the quadrant.
double theArcTangent = Math.atan(Math.radians(45)); //0.6658

### atan2

```flexscript
double (static) Math.atan2(double dy, double dx)
```

Returns the arc tangent of an angle in radians.

**Parameters:**
- `dy` *(double)* — The length of the "opposite" side of a right triangle.
- `dx` *(double)* — The length of the "adjacent" side of a right triangle.

**Returns:** The arc tangent of the angle in radians.

The returned angle will increase from 0 to pi/2 beginning at
the positive x axis in a counter-clockwise direction, and will decrease from 0 to
-pi/2 in a clockwise direction. If both parameters of atan2 are 0,
the function returns 0. atan2 is well defined for every point other than the origin, even if value1 equals
0 and value2 does not equal 0.
double theAngle = Math.degrees(Math.atan2(-1,1.732)); // -30

### ceil

```flexscript
int (static) Math.ceil(double value)
```

Returns the smallest integer value not less than value.

**Parameters:**
- `value` *(double)* — The value to ceiling.

**Returns:** The smallest integer value not less than value.

double myval = Math.ceil(2.3); // 3.
double myval = Math.ceil(3.8); // 4.
double myval = Math.ceil(-2.3); // -2.
double myval = Math.ceil(-3.8); // -3.
See also floor().

### cos

```flexscript
double (static) Math.cos(double angle)
```

Returns the cosine of an angle in radians.

**Parameters:**
- `angle` *(double)* — The angle in radians.

**Returns:** The cosine of the angle in radians.

The angle should be less than 263 radians and greater than -263 radians to avoid loss
of significance in the result.
double theCosine = Math.cos(Math.radians(180)); //-1
See also acos().

### degrees

```flexscript
double (static) Math.degrees(double angle)
```

Converts an angle in radians to degrees.

**Parameters:**
- `angle` *(double)* — The angle in radians.

**Returns:** The angle in degrees.

One radian is 180/pi degrees (~57.2958).
double degrees = Math.degrees(Math.pi); // 180
See also radians().

### exp

```flexscript
double (static) Math.exp(double exponent)
```

Returns (e ^ value) where e is 2.7183

**Parameters:**
- `exponent` *(double)* — The exponent of e.

**Returns:** e raised to an exponent.

On overflow, the function returns INF (infinite) and on underflow, exp returns 0.
double value = Math.exp(3); // 20.0855
See also log() which is the inverse of exp().

### fabs

```flexscript
double (static) Math.fabs(double value)
```

Returns the absolute value of the number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The absolute value of the number.

This code checks if the operator is within 5 units of the current object along the x-axis.
if(Math.fabs(current.location.x - operator.location.x) < 5)

### factorial

```flexscript
double (static) Math.factorial(double value)
```

Returns the factorial of the number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The factorial of the number.

double val = Math.factorial(5); // 120 = 1*2*3*4*5

### floor

```flexscript
int (static) Math.floor(double value)
```

Returns the largest integer value not larger than the number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The largest integer value not larger than the number.

double myval = Math.floor(2.3); // 2.
double myval = Math.floor(3.8); // 3.
double myval = Math.floor(-2.3); // -3.
double myval = Math.floor(-3.8); // -4.
See also ceil() and trunc().

### fmod

```flexscript
double (static) Math.fmod(double numerator, double denominator)
```

Returns the remainder of dividing the numerator by the denominator.

**Parameters:**
- `numerator` *(double)* — The value being divided.
- `denominator` *(double)* — The value to divide by.

**Returns:** The remainder of dividing the numerator by the denominator.

If the denominator is 0.0, fmod returns a quiet NaN.
Math.fmod(10,3) // 1.0
This method is great for getting a repeating set of numbers. So long as the denominator is
unchanged, incrementing the numerator and calling fmod will return a set of numbers that keeps
repeating itself between 0 and 1 minus the denominator. The following condition, if used
in the OnExit of an object in the model, would be true with every 10th flowitem that exits,
starting with the 10th flowitem.
if(Math.fmod(current.stats.output.value,10) == 0)

### frac

```flexscript
double (static) Math.frac(double value)
```

Returns the decimal part of a number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The decimal part of the specified floating point value.

Math.frac(3.124) // 0.124
Math.frac(-16.12) // -0.12
Math.frac(12) // 0.0

### log

```flexscript
double (static) Math.log(double value)
```

Returns the natural logarithm of a number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The natural logarithm of a number.

If value is negative, this function returns an indefinite (same as a quiet NaN). If value is 0,
it returns INF (infinite).
Math.log(9); // 2.1972
log() is the inverse of the exp() method.
Math.log(Math.exp(3.2)); // 3.2

### log10

```flexscript
double (static) Math.log10(double value)
```

Returns the base-10 logarithm of a number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The base-10 logarithm of a number.

If value is negative, this function returns an indefinite (same as a quiet NaN). If value is 0,
it returns INF (infinite).
Math.log10(1000); // 3
log10() is the inverse of the pow() method with base 10.
Math.log10(Math.pow(10, 3.2)); // 3.2

### max

```flexscript
double (static) Math.max(double value1, double value2)
```

Returns the greater of two numbers, or the max value of an array.

**Parameters:**
- `value1` *(double)* — The first value to compare.
- `value2` *(double)* — The second value to compare.

**Returns:** The greater of two numbers, or the max value of an array.

double max = Math.max(6,7); // 7
double max = Math.max([3,9,4]); // 9
The following code will generate a normally distributed random number between and inclusive of 0 and 100:
double haulLoad = Math.min(Math.max(normal(55, 27), 0), 100);
See also min().

### min

```flexscript
double (static) Math.min(double value1, double value2)
```

Returns the lesser of two numbers, or the min value of an array.

**Parameters:**
- `value1` *(double)* — The first value to compare.
- `value2` *(double)* — The second value to compare.

**Returns:** The lesser of two numbers, or the min value of an array.

double min = Math.min(6,7); // 6
double max = Math.min([3,9,4]); // 3
The following code will generate a normally distributed random number between and inclusive of 0 and 100:
double haulLoad = Math.min(Math.max(normal(55, 27), 0), 100);
See also max().

### pow

```flexscript
double (static) Math.pow(double base, double exponent)
```

Returns the base raised to the power of the exponent.

**Parameters:**
- `base` *(double)* — The value being raised to a power.
- `exponent` *(double)* — The exponent the base will be raised to.

**Returns:** The base raised to the power of the exponent.

double myval = Math.pow(2,3); // 8.

### radians

```flexscript
double (static) Math.radians(double angle)
```

Converts an angle in degrees to radians.

**Parameters:**
- `angle` *(double)* — The angle in degrees.

**Returns:** The angle in radians.

1 degree is pi/180 radians.
double radians = Math.radians(180); // 3.1415 (pi)
See also degrees().

### round

```flexscript
double (static) Math.round(double value, int precision = 0)
```

Rounds the value to the specified precision.

**Parameters:**
- `value` *(double)* — The value being rounded.
- `precision` *(int, default 0)* — The number of decimal places to round to.

**Returns:** The value rounded to the specified precision.

A fractional part 0 < f < 0.5 will be rounded down. A fractional part 0.5 <= f < 1.0
will be rounded up. Using a precision of 0 (or not specifying it) will round to the nearest
integer.
int myNum = Math.round(3.6269); // 4
int myNum = Math.round(3.6269, 2); // 3.63

### sign

```flexscript
int (static) Math.sign(double value)
```

Returns the sign of the number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The sign of the number.

int sign = Math.sign(-100); // -1
int sign = Math.sign(100); // 1
int sign = Math.sign(0); // 0

### sin

```flexscript
double (static) Math.sin(double angle)
```

Returns the sine of an angle in radians.

**Parameters:**
- `angle` *(double)* — The angle in radians.

**Returns:** The sine of the angle in radians.

The angle should be less than 263 radians and greater than -263 radians to avoid loss
of significance in the result.
double theSine = Math.sin(Math.radians(90)); // 1
See also asin().

### sqr

```flexscript
double (static) Math.sqr(double value)
```

Returns the number squared.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The number squared.

int square = Math.sqr(4); // 16
See also sqrt().

### sqrt

```flexscript
double (static) Math.sqrt(double value)
```

Returns the square root of the number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The square root of the number.

int root = Math.sqrt(25); // 5
See also sqr().

### tan

```flexscript
double (static) Math.tan(double angle)
```

Returns the tangent of an angle in radians.

**Parameters:**
- `angle` *(double)* — The angle in radians.

**Returns:** The tangent of the angle in radians.

The angle should be less than 263 radians and greater than -263 radians to avoid loss
of significance in the result.
double theTangent = Math.tan(Math.radians(45)); // 1
See also atan().

### trunc

```flexscript
int (static) Math.trunc(double value)
```

Removes the fractional part of the number.

**Parameters:**
- `value` *(double)* — The passed in value.

**Returns:** The fractional part of the number.

Truncates the value.
int val = Math.trunc(2.478); // 2
int sign = Math.sign(2.95); // 2
See also round().

## Properties

### pi

```flexscript
double Math.pi
```

Gets an approximate value for pi.

The internal representation is 3.14159265358979.
Math.cos(Math.pi) // -1

