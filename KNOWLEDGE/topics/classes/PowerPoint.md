---
id: powerpoint
name: "PowerPoint"
kind: class
module: ""
signature: "PowerPoint"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class for accessing PowerPoint."
seeAlso: []
source: "help\\FlexScriptAPIReference\\PowerPoint\\PowerPoint.xml"
---

# PowerPoint

A class for accessing PowerPoint.

Class references for creating, opening, and editing PowerPoint presentations, slides, and shapes.
PowerPoint.Presentation
PowerPoint.Slide
PowerPoint.Shape
PowerPoint.Layout
PowerPoint.ShapeType
PowerPoint.SlideSizeType
Example code for how to create a basic PowerPoint Presentation.

PowerPoint.Presentation presentation = PowerPoint.Presentation.open(modeldir() + "\\Presentation.pptx", 1);
PowerPoint.Slide Slide = Presentation.addSlide(PowerPoint.Layout.title);
PowerPoint.Shape Title = Slide.shapes[1];
PowerPoint.Shape subTitle = Slide.shapes[2];
Title.text = "The Presentation's Title in Blue";
Title.color = Color.blue;
subTitle.text= "The Presentation's Subtitle in Green";
subTitle.color = Color.green;
Presentation.save(documentsdir() + "\\Presentation.pptx");
Presentation.close();

