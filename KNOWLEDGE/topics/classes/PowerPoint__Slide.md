---
id: powerpoint__slide
name: "PowerPoint::Slide"
kind: class
module: "PowerPoint"
signature: "PowerPoint::Slide"
aliases: []
tags: ["class", "powerpoint-module"]
deprecated: false
replacedBy: null
description: "A class for managing and editing PowerPoint slides."
seeAlso: []
source: "help\\FlexScriptAPIReference\\PowerPoint\\PowerPoint.Slide.xml"
---

# PowerPoint::Slide

A class for managing and editing PowerPoint slides.

## Methods

### addMedia

```flexscript
PowerPoint.Shape PowerPoint::Slide.addMedia(string filePath, Vec2 position, Vec2 size)
```

Adds media to the slide, can be a sound or video file.

**Parameters:**
- `filePath` *(string)* — The path to the media file you want to add.
- `position` *(Vec2)* — The position of the media object, top left corner is 0, 0.
- `size` *(Vec2)* — The size of the media object.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addMedia(modeldir() + "\\SampleVideo.mp4", Vec2(80,80),Vec2(200,200));

### addPicture

```flexscript
PowerPoint.Shape PowerPoint::Slide.addPicture(string filePath, Vec2 position, Vec2 size)
```

Adds a picture to the slide.

**Parameters:**
- `filePath` *(string)* — The path to the picture file you want to add.
- `position` *(Vec2)* — The position of the picture object, top left corner is 0, 0.
- `size` *(Vec2)* — The size of the picture object.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addPicture(modeldir() + "\\SamplePicture.png", Vec2(80,80),Vec2(200,200));

### addTable

```flexscript
PowerPoint.Shape PowerPoint::Slide.addTable(Table table, Vec2 position, Vec2 size, double fontSize = 18)
```

Adds a table to the slide from a table in FlexSim.

**Parameters:**
- `table` *(Table)* — The Table the data will be pulled from.
- `position` *(Vec2)* — The position of the table, top left corner is 0, 0.
- `size` *(Vec2)* — The size of the table. If the size is smaller than the space required to fill the table, it will autosize to accommodate.
- `fontSize` *(double, default 18)* — The size of the font.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addMedia(Table("GlobalTable1"), Vec2(80,80),Vec2(200,200), 12);

### addTextBox

```flexscript
PowerPoint.Shape PowerPoint::Slide.addTextBox(string text, Vec2 position, Vec2 size, double fontSize = 18, int bold = 0, Color color = Color.black)
```

Adds a textbox to the slide.

**Parameters:**
- `text` *(string)* — The text you want to be displayed.
- `position` *(Vec2)* — The position of the textbox, top left corner is 0, 0.
- `size` *(Vec2)* — The size of the textbox. The textbox will autosize to accommodate text that occupies a larger or smaller space then specified.
- `fontSize` *(double, default 18)* — The size of the font.
- `bold` *(int, default 0)* — Pass in a 1 to bold the text.
- `color` *(Color, default Color.black)* — Set the color of the text.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].addTextBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);

### assertTitleBox

```flexscript
PowerPoint.Shape PowerPoint::Slide.assertTitleBox(string text, Vec2 position, Vec2 size, double fontSize = 18, int bold = 0, Color color = Color.black)
```

Asserts a title text box if the slide layout includes or included one.

**Parameters:**
- `text` *(string)* — The text you want to be displayed.
- `position` *(Vec2)* — The position of the textbox, top left corner is 0, 0.
- `size` *(Vec2)* — The size of the textbox. The textbox will autosize to accommodate text occuping a larger or smaller space than specified.
- `fontSize` *(double, default 18)* — The size of the font.
- `bold` *(int, default 0)* — Pass in a 1 to bold the text.
- `color` *(Color, default Color.black)* — Set the color of the text.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.title);
presentation.slides[1].assertTitleBox("New Text", Vec2(80, 80), Vec2(200, 50), 30, 0, Color.blue);

### delete

```flexscript
void PowerPoint::Slide.delete()
```

Deletes the slide.

PowerPoint.Presentation presentation;
presentation.open(modeldir() + "\\Presentation.pptx", 1);
presentation.slides[1].delete();

### duplicate

```flexscript
PowerPoint.Slide PowerPoint::Slide.duplicate()
```

duplicates the slide.

PowerPoint.Presentation presentation = PowerPoint.Presentation.open(modeldir() + "\\Presentation.pptx", 1);
presentation.slides[1].duplicate();

### setBackgroundColor

```flexscript
void PowerPoint::Slide.setBackgroundColor(Color color)
```

Sets the slide's background color.

**Parameters:**
- `color` *(Color)* — The color you are setting the background color to.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slides[1].setBackgroundColor(Color(1, 0, 0, .5)); //Sets the background color of the slide to Salmon.

## Properties

### backgroundColor

```flexscript
Color PowerPoint::Slide.backgroundColor
```

Gets/sets the slide's background color.

PowerPoint.Presentation presentation;
presentation.open(modeldir() + "\\Presentation.pptx", 1);
presentation.slides[1].backgroundColor = Color.red;
return presentation.slides[1].backgroundColor;

### index

```flexscript
int PowerPoint::Slide.index
```

Gets/sets the index of the slide in the presentation.

PowerPoint.Presentation presentation;
presentation.create(1);
PowerPoint.Slide Slide1 = presentation.addSlide(PowerPoint.Layout.blank);
PowerPoint.Slide Slide2 = presentation.addSlide(PowerPoint.Layout.blank);
print(Slide1.index); //1
Slide2.index = 1; //The second slide is now the first slide in the presentation
return Slide1.index; //2

### shapes

```flexscript
PowerPoint.Shape PowerPoint::Slide.shapes (readonly)
```

Accesses shapes in the slide (Read Only).

Accessing the Number of shapes in the presentation
You can access the number of shapes in the presentation with the following code.
slide.shapes.length
Accessing shapes by Rank
You can access an individual shape with the following code.

slide.shapes[1] // first shape
slide.shapes[slide.shapes.length] // last shape
slide.shapes[i] // i-th shape

