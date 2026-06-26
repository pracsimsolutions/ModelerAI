---
id: reference_propertiespanels_visualpanels_light_light
name: "Light"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","VisualPanels","Light","Light"]
tags: ["manual","reference","propertiespanels","visualpanels"]
source: "manual/Reference/PropertiesPanels/VisualPanels/Light/Light.html"
---

# Light

The Light panel defines how the light shines.

 

 

The following properties are on the Light panel:

 

### Type

 

The type of light determines how the light affects the scene.

 

 

- **Directional** - Directional lights apply to each object from a single direction, like the sun. The rotation of the light affects the direction. The position and size of the light are unused.
- **Point** - Point lights emit from a specific point towards each object, like a lamp. The position of the light controls its origin. The rotation and size of the light are unused.
- **Area** - Area lights emit from a rectangular area in a specific direction. The position, rotation, and size of the light control its area of influence. In RTX Mode, the rectangular area is used directly. Out of RTX Mode, the center of the rectangle emits light in a radius influenced by the area of the rectangle.

 

### Range

 

The range controls the distance that a point light influences. For an area light, this value is calculated based on the size of the light rather than directly edited. Directional lights have infinite range.

 

### Intensity

 

The intensity controls the brightness of the light. The color of the light will be multiplied by the intensity value when calculating the influence of the light.

 

### Radius

 

In RTX Mode, the source radius of a point light will affect the softness of shadows cast by the light. Greater values will make the shadows softer. A value of 0 will cast hard shadows.

 

### Angle

 

In RTX Mode, the angular size (in degrees) of a directional light will affect the softness of shadows cast by the light. Greater values will make the shadows softer. A value of 0 will cast hard shadows.
