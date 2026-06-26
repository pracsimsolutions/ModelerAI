---
id: reference_tools_colorpalette_colorpalette
name: "Color Palette"
kind: manual
breadcrumb: ["Reference","Tools","ColorPalette","ColorPalette"]
tags: ["manual","reference","tools","colorpalette"]
source: "manual/Reference/Tools/ColorPalette/ColorPalette.html"
---

# Color Palette

## Overview and Key Concepts

 

A Color Palette is a list of colors. Each color can be associated with a value. You can then look up a color using the value associated with that color, using the Color.fromPalette() method.

 

 

You can access Color Palettes in the toolbox.

 

### Assigning Values to Colors

 

In order to access a color from a Color Palette, you must first assign a value to that color. The value can one of the following types of values: 

- number
- text
- node
- an array containing number, text, or node values

 

### Accessing Colors

 

You can access a palette's colors through the Color.fromPalette() method. The most common way to use this method is to pass in a value and the name of a palette. The palette will look up the color associated with the given value, and return that color.

 

If the palette does not have a color assigned to the given value, then the palette will look for any colors that don't have a value assigned yet. If one exists, then the palette will assign the color to the given value and return the color. If there are no unused colors, then the color palette will generate a color, assign the given value to the new color, and return the new color.

 

### Using a Gradient

 

A gradient allows you to assign colors to continuous values:

 

 

The gradient is defined by the set of colors that are assigned to number values. Each of these colors becomes a color stop in the gradient. If you call Color.fromPalette(), passing in a number value and a gradient palette, then that palette will interpolate between assigned colors to return the correct color. The following table shows the colors that would be returned from the palette shown above:

 

| Code | Returned Color |
| --- | --- |
| `Color.fromPalette(5.5, "GradientPalette")` |  |
| `Color.fromPalette(18, "GradientPalette")` |  |
| `Color.fromPalette(32.3, "GradientPalette")` |  |
| `Color.fromPalette(102, "GradientPalette")` |  |
| `Color.fromPalette(88, "GradientPalette")` |  |
| `Color.fromPalette(-5, "GradientPalette")` |  |

 

When a palette interpolates to find a color, it does not assign that value to the interpolated color. This way, the list of colors remains constant.

  

#### Gradients apply to all number values

 

If you use a gradient on a palette, all number values will be involved in that gradient. All non-number values will behave as if there is no gradient being used.

  

 

 

## Properties

 

The Color Palette has the following properties:

 

 

### Name

 

You can use this property to edit the name of the palette. You can also use this property to change which palette you are viewing.

 

### Use Gradient

 

If you check this box, the color palette will use the assigned number values to form a gradient. A gradient editor field will appear, allowing you to change the locations of the color stops.

 

### Edit Values

 

This button opens a menu. The menu contains options to edit many or all colors and values simultaneously.

 

### Color List

 

This property shows the list of colors known to the palette, and also which value is assigned to each color. This list allows you to perform the following operations: 

- You can edit the value assigned to each color.
- You can edit the given color.
- You can remove a color.
- You can reorder the colors. This has no effect on the Color Palette's logic, but it can sometimes be helpful to group certain colors/values together.
