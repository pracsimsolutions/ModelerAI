---
id: reference_processflowobjects_display_text_text
name: "Text"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Display","Text","Text"]
tags: ["manual","reference","processflowobjects","display"]
source: "manual/Reference/ProcessFlowObjects/Display/Text/Text.html"
---

# Text

## Overview and Key Concepts

 

The Text object is a multiline custom text box that can be placed anywhere inside a process flow. The Text object is for display only, meaning that it doesn't have any other function or purpose other than visual display.

 

  

#### Inserting Line Breaks

 

You can insert a line break by pressing Ctrl-Enter.

  

 

 

## Connectors

 

Text objects do not allow incoming or outgoing connectors. They are for visual use only.

 

 

 

## Properties

 

The following image shows properties for Text Display Objects:

 

 

Each of these settings will be explained in the following sections.

 

### Name

 

You can use the **Name** box to change the name of the text object.

 

### Lock To

 

You can use the **Lock To** box to attach the Text object to an activity in your process flow. When the Text is locked to an object, it will move when that activity is moved. Use the **Sampler** button to select the activity the Text object should be locked with.

 

### Rotation

 

Using the **Rotation** box, you can set the angle (in degrees) at which the Text object will display. The base line is horizontal and the rotation is to the right. For example, a rotation value of 90 will result in the following image:

 

 

### Height, Width

 

Adjust the size of the text object by setting the height or width of the box.

 

### Shrink Text on Overflow

 

When checked, the font size of the text will automatically be decreased if the contents are too large to fit inside the text box.

 

### Billboard

 

When checked, the text object will maintain its relative position in the process flow window and won't move when the view is scrolled.

 

### Dynamic Text

 

When checked, it will cause the Text Object to display dynamic values during a simulation run. The Text object can, for example, be set to display the value of a process flow variable or label. You can possibly use these variables or labels to display relevant data or statistics during a simulation run.

 

### Font

 

There are a variety of font properties you can use to change the text display:

 

- The font menu has 13 common fonts available.
- You can change the font size.
- You can make the text bold, italicized, or underlined.
- You can change the color of the text by using the color selector or using the **Sampler** button to select a color from any object in your simulation model or process flow.
