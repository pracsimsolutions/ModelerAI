---
id: reference_tools_conveyorsystemtool_conveyorvisualizations_conveyorvisualizations
name: "Conveyor Visualizations"
kind: manual
breadcrumb: ["Reference","Tools","ConveyorSystemTool","ConveyorVisualizations","ConveyorVisualizations"]
tags: ["manual","reference","tools","conveyorsystemtool"]
source: "manual/Reference/Tools/ConveyorSystemTool/ConveyorVisualizations/ConveyorVisualizations.html"
---

# Conveyor Visualizations

## Overview and Key Concepts

 

Conveyor visualizations are global settings that are referenced by model conveyors. They define how the conveyors should look.

 

 

 

## Properties

 

Visual settings mostly define how conveyors look when drawing in render mode. Render mode can be enabled in Conveyor System Properties. The properties window has the following properties:

 

 

## Surface

 

The surface pane allows you to define visual properties associated with the convey surface.

 

### Draw Rollers

 

Check this box to enable the drawing of rollers on the conveyor surface.

 

### Diameter

 

The diameter to draw rollers when in render mode. Only used in render mode.

 

### Spacing

 

The distance from the center of one roller to the center of the next roller. Should be greater than Roller Diameter. Only used in render mode.

 

### Roller Color

 

The color to apply when drawing the rollers. Can be set to use the object's color.

 

### Draw Belt

 

Check this box to enable the drawing of a belt on the conveyor surface. Then you can define the texture to be drawn. There are several automatic options, or you can browse for the file.

 

### X Repeat

 

The distance between each repeat of the conveyor belt texture, along the conveyor's forward axis.

 

### Y Repeat

 

The repeat value for the conveyor belt texture, along the conveyor's left-right axis. You also define a mode for repeating, either *Stretch* or *Tile*. In *Stretch* mode, the repeat value represents the number of times the texture will repeat, independent of the conveyor width. For *Tile*, the repeat value represents a repeat distance, just like the X Repeat.

 

### Belt Color

 

The color to apply when drawing the belt texture. Can be set to use the object's color.

 

## Side Skirt

 

The side skirt pane allows you to define visual properties associated with drawing the conveyor's side skirts.

 

### Height

 

The height of the conveyor's side skirt.

 

### Width

 

The width of the side skirt. Only used in render mode.

 

### Vertical Offset

 

Defines the position of the side skirt relative to the conveyor plane. This is defined as a ratio of the Side Skirt Height. Usually the value will be between -0.5 and 0.5. For a value of -0.5, the side skirt will extend down from the conveyor plane. For a value of 0.0, the side skirt's center will follow the conveyor plane. For a value of 0.5, the side skirt will extend up from the conveyor plane. 

### Draw Texture

 Check this box to draw a texture for the side skirt, instead of drawing a pre-defined shape. Then you can define what texture to use. There are some pre-defined options, or you can browse for an image.

 

### Repeat Distance

 

Check this box to define repeat distance associated with the side skirt. This applies both if you are using a side skirt texture, or if you want the conveyor to draw its pre-defined side skirt shape.

 

### Side Skirt Color

 

Defines the color to apply when drawing the side skirt. When in non-render mode, this applies to both the side skirt and the conveyor surface. Can be set to use the object's color.

 

## Legs

 

The legs pane allows you to define visual properties associated with drawing the conveyor's legs.

 

### Repeat Distance

 

The distance between each leg of the conveyor. Only used in render mode.

 

### Height Rule

 

Defines how the height of each leg will be determined. Only used in render mode.

 

- **Leg Height Following Conveyor Height** - Legs will be a set height no matter how the conveyor rises or falls. If the conveyor rises, leg bases will rise with it.
- **Leg Height from Lowest Point on Conveyor** - Leg height is determined from the lowest point on the conveyor. For example, if the conveyor plane goes from z = 1.00 to z = 5.00, and the Leg Height is defined as 1.00, the leg base will be at z = 0.00 all along the conveyor (lowest point (1.0) minus leg height (1.0)).
- **Leg Base Relative to Model** - Legs will extend to a fixed model z position, defined by the Leg Base, no matter how high the conveyor is.

 

### Height

 

The conveyor leg height. Only used in render mode when Leg Height Rule equals Leg Height Following Conveyor Height or Leg Height From Lowest Point on Conveyor.

 

### Base

 

The leg base z-point. Only used in render mode when Leg Height Rule equals Leg Base Relative to Model.
