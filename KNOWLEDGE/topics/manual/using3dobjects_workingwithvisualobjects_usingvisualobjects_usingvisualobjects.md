---
id: using3dobjects_workingwithvisualobjects_usingvisualobjects_usingvisualobjects
name: "Using Visual Objects"
kind: manual
breadcrumb: ["Using3DObjects","WorkingWithVisualObjects","UsingVisualObjects","UsingVisualObjects"]
tags: ["manual","using3dobjects","workingwithvisualobjects","usingvisualobjects"]
source: "manual/Using3DObjects/WorkingWithVisualObjects/UsingVisualObjects/UsingVisualObjects.html"
---

# Using Visual Objects

## Overview of Visual Objects

 

The visual objects that are available in the FlexSim library can help you add visual details that might be useful when presenting the model to stakeholders.

 

The following table provides an overview of the purpose and common uses for each visual object. The objects are listed from the most commonly used to the least commonly used:

 

| Object | Purpose |
| --- | --- |
| Text | Use a text object if you want to add text to a specific place in your model. Text objects can be helpful for clearly labeling portions of the model when you share the model with stakeholders or people who are working on the same model. |
| Billboard | The billboard object is very similar to the text object. The difference is that a billboard object will stay in a specific place on the model view screen; it will not move with the model as you pan or zoom. |
| Plane | Planes can act as a model floor, which is useful if you want to simulate a location with multiple floors. See Creating Model Layouts for more information. |
| Slide | Similar to text, you can use a slide to add a block of text or other information from the model. Slides are similar in appearance to presentation slides that you might create in a program such as PowerPoint. |
| Shape | Use shapes if you want to add a 3D shape such as a cube or a sphere to the model. Another good use for shapes is that they can act as locations that customers or employees can travel to. |
| Background | The background is actually a tool, not a specific object per se. You can use this tool to add a floor plan or CAD drawing to your model. See Importing CAD Drawings and Floor Plans for more information. |
| Walls | Walls are not just one object. Clicking on walls allows pillars to be placed and connected together. This can be used to create a complex walls structure. This object can also be added as an A* Member to use walls as A* dividers. |
| Light | Use light objects to customize the lighting of your model. If the model contains light objects, then they will be used instead of the default light settings on the view. |

  

#### The Model Floor

 

The model floor is also a visual object, although it is not found in the Library. See Changing the Appearance of the Model Floor for more information.

   

#### Creating Custom Shapes

 

You can create your own custom 3D shapes if desired. See Creating Custom 3D Objects for more information.

  

 

 

## Properties

 

Visual Tool objects use the following properties panels:

 

- Statistics
- Visuals
- Labels
- Text (Text and Billboard objects only)
- Texture (Plane, Slide, and Shape objects only)
- VisualTool (Except Light and Walls objects)
- Light (Light object only)
- Wall (Wall object only)
- Ports
- Triggers

 

 

 

## Changing the Text in Slide Objects

 

Slide objects are designed to look like a presentation slide. Slides consist of several text objects that are displayed on a single plane.

 

 

### Adding Text Objects

 

To add text objects to the slide object drag a Text object from the library onto the slide object.

 

### Removing Text Objects

 

To remove text objects from the slide object click on the desired text object and then press the delete key.

 

### Changing the Text in Slide Objects

 

To change the text in slide objects, select the desired text object and then edit its text in the Text panel in the Properties window.

 

 

 

## Changing the Appearance of the Model Floor

 

You can change the appearance of the model floor if needed. You can either add a texture to the floor or change the color.

 

To change the model floor settings:

 

1. With the Toolbox open, click the **Add** button to open a menu. Point to **Visual**, then select **Model Floor** to open the Model Floor settings.
1. Use the **Color** box to select a different color for the model floor.
1. Use the **Texture** box to use a texture file (an image) for the model floor instead. You can also select one of the textures that come with FlexSim such as tiles, concrete, or dirt.
1. Use the **Polygon Offset** box to define the polygon offset that the floor should be rendered with.
