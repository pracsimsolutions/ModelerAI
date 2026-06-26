---
id: color
name: "Color"
kind: class
module: ""
signature: "Color"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class containing rgba values for an object's color."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Data\\Color.xml"
---

# Color

A class containing rgba values for an object's color.

## Methods

### byNumber

```flexscript
Color (static) Color.byNumber(int number)
```

Returns a color by a uniquely identifying number.

**Parameters:**
- `number` *(int)* — The identifying number.

**Returns:** The resulting color.

item.color = Color.byNumber(item.labelVal);
The number passed in refers to an index in the standard colors spectrum, as follows:
1 - red
2 - green
3 - blue
4 - yellow
5 - orange
6 - purple
7 - lime
8 - pink
9 - gray
10 - teal
11 - aqua
12 - brown
13 - light blue
14 - silver
15 - white
For numbers above 15, a unique color will generated based on an algorithm.

### fromPalette

```flexscript
Color (static) Color.fromPalette(Variant value, Variant palette = nullvar)
```

Returns a color from a color palette.

**Parameters:**
- `value` *(Variant)* — The value to get a color for.
- `palette` *(Variant, default nullvar)* — The Color Palette to get a color from.

**Returns:** The resulting color.

A palette can either be specified by name or
by node. The color palette will look up the color
assigned to the given value, asserting a color for the
value if necessary. However, if the palette
is using a gradient, the palette will linearly interpolate
between the color values already defined by the palette.
If no palette is specified, then the resulting color will
be generated using the given value.

### random

```flexscript
Color (static) Color.random(int stream = 0)
```

Returns a random color.

**Parameters:**
- `stream` *(int, default 0)* — The random number stream to use.

**Returns:** The resulting color.

current.color = Color.random();

### Color

```flexscript
Color.Color(double r, double g, double b, double a = 1)
```

Constructs a color by rgba component values.

**Parameters:**
- `r` *(double)* — The red component, in the range 0-1.
- `g` *(double)* — The green component, in the range 0-1.
- `b` *(double)* — The blue component, in the range 0-1.
- `a` *(double, default 1)* — The alpha component, in the range 0-1.

current.color = Color(0.1, 0.6, 0.3);

### lerp

```flexscript
Color Color.lerp(Color targetColor, double t)
```

Linearly interpolates between two colors.

**Parameters:**
- `targetColor` *(Color)* — The color to interpolate to.
- `t` *(double)* — A ratio between 0 and 1 that defines the percentage to interpolate from the current color to targetColor.

**Returns:** The resulting interpolated color.

The following example code changes current's color 20% toward red.
current.color = current.color.lerp(Color.red, 0.2);

## Properties

### a

```flexscript
double Color.a
```

The color's alpha component.

Color components are in the range 0-1

### aqua

```flexscript
Color Color.aqua (readonly)
```

An aqua color

current.color = Color.aqua;

### b

```flexscript
double Color.b
```

The color's blue component.

Color components are in the range 0-1

### black

```flexscript
Color Color.black (readonly)
```

A black color

current.color = Color.black;

### blue

```flexscript
Color Color.blue (readonly)
```

A blue color

current.color = Color.blue;

### brown

```flexscript
Color Color.brown (readonly)
```

A brown color

current.color = Color.brown;

### g

```flexscript
double Color.g
```

The color's green component.

Color components are in the range 0-1

### gray

```flexscript
Color Color.gray (readonly)
```

A gray color

current.color = Color.gray;

### green

```flexscript
Color Color.green (readonly)
```

A green color

current.color = Color.green;

### lightBlue

```flexscript
Color Color.lightBlue (readonly)
```

A light blue color

current.color = Color.lightBlue;

### lime

```flexscript
Color Color.lime (readonly)
```

A lime color

current.color = Color.lime;

### orange

```flexscript
Color Color.orange (readonly)
```

An orange color

current.color = Color.orange;

### pink

```flexscript
Color Color.pink (readonly)
```

A pink color

current.color = Color.pink;

### purple

```flexscript
Color Color.purple (readonly)
```

A purple color

current.color = Color.purple;

### r

```flexscript
double Color.r
```

The color's red component.

Color components are in the range 0-1

### red

```flexscript
Color Color.red (readonly)
```

A red color

current.color = Color.red;

### silver

```flexscript
Color Color.silver (readonly)
```

A silver color

current.color = Color.silver;

### teal

```flexscript
Color Color.teal (readonly)
```

A teal color

current.color = Color.teal;

### white

```flexscript
Color Color.white (readonly)
```

A white color

current.color = Color.white;

### yellow

```flexscript
Color Color.yellow (readonly)
```

A yellow color

current.color = Color.yellow;

