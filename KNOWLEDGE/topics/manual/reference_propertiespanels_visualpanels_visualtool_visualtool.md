---
id: reference_propertiespanels_visualpanels_visualtool_visualtool
name: "Visual Tool"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","VisualPanels","VisualTool","VisualTool"]
tags: ["manual","reference","propertiespanels","visualpanels"]
source: "manual/Reference/PropertiesPanels/VisualPanels/VisualTool/VisualTool.html"
---

# Visual Tool

The Visual Tool panel defines what texture is draw on the plane.

 

 

The following properties are on the Visual Tool panel:

 

### Visual Display

 

Choose a shape for this visual tool.

 

- **Plane**
- **Cube**
- **Column**
- **Sphere**
- **Imported Shape**
- **Text**
- **Presentation Slide**

 

### Billboard Mode

 

Select whether the object is in billboard mode. There are a few options:

 

- **No Billboard** - Turns off billboard mode.
- **Vertical Billboard** - The plane will rotate around the Z axis to always face towards the screen.
- **All Around Billboard** - The plane will rotate around 2 axes so that it is always facing directly towards the screen.
- **Screen Locked Billboard** - The object will not move when you navigate around the model and will instead stay locked to a certain position on the screen.

 

### Luminous

 

Turns lighting effects off for this object which makes it appear to be emitting its own light.

 

### No Select

 

The object will not be selectable in the model.

 

### Polygon Offset Factor/Units

 

Only applicable in *Plane* mode (or as a model floor). This defines the polygon offset to use when drawing the plane. Polygon offset is used to resolve [Z-fighting](https://en.wikipedia.org/wiki/Z-fighting) between objects. The *Factor* and *Units* values you specify are passed directly into [glPolygonOffset()](https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glPolygonOffset.xhtml).

 

Generally for a visual tool, you want to define a positive polygon offset. This means the depth buffer value will be increased, i.e. pushed away from the camera, allowing other geometry, like the floor grid, connectors, etc. to be drawn on top of the visual tool plane. This is why the default Factor/Units values are 1/1. If you want to fix z fighting across multiple visual tools, give them different offset values. For example, give one the default 1/1 value, and the other a 2/2 value. This will cause the 2/2 valued object to be drawn behind the 1/1 valued object.

 

### Pass Inputs/Outputs To

 

If the object is used as a container for other objects, then the properties panel will show fields for passing the object's input/output connections to some contained object. Here you define the name of the target contained object. Then when you make input/output connections to/from the container, internal connections will also be created to the contained object.
