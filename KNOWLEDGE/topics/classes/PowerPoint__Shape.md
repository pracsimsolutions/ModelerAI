---
id: powerpoint__shape
name: "PowerPoint::Shape"
kind: class
module: "PowerPoint"
signature: "PowerPoint::Shape"
aliases: []
tags: ["class", "powerpoint-module"]
deprecated: false
replacedBy: null
description: "A class for managing and editing PowerPoint shapes."
seeAlso: []
source: "help\\FlexScriptAPIReference\\PowerPoint\\PowerPoint.Shape.xml"
---

# PowerPoint::Shape

A class for managing and editing PowerPoint shapes.

## Methods

### delete

```flexscript
PowerPoint::Shape.delete()
```

Deletes the shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.open(modeldir() + "\\Presentation.pptx", 1);
presentation.slides[1].shapes[1].delete();

### duplicate

```flexscript
PowerPoint.Shape PowerPoint::Shape.duplicate()
```

Duplicates the shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);
presentation.slides[1].shapes[1].duplicate();

## Properties

### bold

```flexscript
int PowerPoint::Shape.bold
```

Gets/sets the font size of a textbox shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);
presentation.slides[1].shapes[1].fontSize = 23.5;

### color

```flexscript
Color PowerPoint::Shape.color
```

Gets/sets the color of the text of a textbox shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);
return presentation.slides[1].shapes[1].color;

### fontSize

```flexscript
double PowerPoint::Shape.fontSize
```

Gets/sets the font size of a textbox shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);
pesentation.slides[1].shapes[1].fontSize = 23.5;

### position

```flexscript
Vec2 PowerPoint::Shape.position
```

Gets/sets the position a shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);
return presentation.slides[1].shapes[1].position;

### size

```flexscript
Vec2 PowerPoint::Shape.size
```

Gets/sets the size a shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addPicture(modeldir() + "\\SamplePicture.png", Vec2(80,80),Vec2(200,200));
return presentation.slides[1].shapes[1].size;

### text

```flexscript
string PowerPoint::Shape.text
```

Gets/sets the text of a textbox shape.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);
presentation.slides[1].shapes[1].text = "New Text Replaced";

### type

```flexscript
int PowerPoint::Shape.type
```

Returns the shapetype of the shape, an enumeration of PowerPoint.ShapeType.

PowerPoint.Presentation presentation = PowerPoint.Presentation.open(modeldir() + "\\Presentation.pptx", 1);
return presentation.slides[1].shapes[1].type;

