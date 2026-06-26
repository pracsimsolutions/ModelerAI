---
id: powerpoint__presentation
name: "PowerPoint::Presentation"
kind: class
module: "PowerPoint"
signature: "PowerPoint::Presentation"
aliases: []
tags: ["class", "powerpoint-module"]
deprecated: false
replacedBy: null
description: "A class for creating, opening, and editing PowerPoint presentations."
seeAlso: []
source: "help\\FlexScriptAPIReference\\PowerPoint\\PowerPoint.Presentation.xml"
---

# PowerPoint::Presentation

A class for creating, opening, and editing PowerPoint presentations.

## Methods

### create

```flexscript
void (static) PowerPoint::Presentation.create(int visible)
```

Create a new empty presentation.

**Parameters:**
- `visible` *(int)* — Have the created presentation visible to the user or not.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);

### open

```flexscript
void (static) PowerPoint::Presentation.open(string filePath, int visible)
```

Opens a presentation from a given path.

**Parameters:**
- `filePath` *(string)* — The path to the PowerPoint presentation you want to open.
- `visible` *(int)* — Have the opened presentation visible to the user or not.

PowerPoint.Presentation presentation = PowerPoint.Presentation.open(modeldir() + "\\Presentation.pptx", 1);

### addSlide

```flexscript
void PowerPoint::Presentation.addSlide(int layout)
```

Adds a slide with a specific layout.

**Parameters:**
- `layout` *(int)* — The layout you want to apply to the slide when it is added. This should be an enumeration of PowerPoint.Layout.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);

### applyTemplate

```flexscript
void PowerPoint::Presentation.applyTemplate(string filePath)
```

Applies a Template from a file.

**Parameters:**
- `filePath` *(string)* — The path to where the theme is.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.applyTemplate(modeldir() + "\\Template.potx");

### applyTheme

```flexscript
void PowerPoint::Presentation.applyTheme(string filePath)
```

Applies a theme from a file.

**Parameters:**
- `filePath` *(string)* — The path to where the theme is.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.applyTheme(modeldir() + "\\Theme.thmx");

### close

```flexscript
void PowerPoint::Presentation.close()
```

Closes the presentation and quits the current PowerPoint instance.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.save(modeldir() + "\\Presentation.pptx");
presentation.close();

### save

```flexscript
void PowerPoint::Presentation.save(string filePath = 0)
```

Saves the presentation.

**Parameters:**
- `filePath` *(string, default 0)* — The path to where you want to save the presentation. If not specified, then it will save over the file path the presentation was opened from or previously saved to.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.save(modeldir() + "\\Presentation.pptx");

## Properties

### slides

```flexscript
PowerPoint.Slide PowerPoint::Presentation.slides (readonly)
```

Accesses slides in the presentation (Read Only).

Accessing the Number of slides in the presentation
You can access the number of slides in the presentation with the following code.
presentation.slides.length
Accessing slides by Rank
You can access an individual slide with the following code.

presentation.slides[1] // first slide
presentation.slides[presentation.slides.length] // last slide
presentation.slides[i] // i-th slide

### slideSize

```flexscript
Vec2 PowerPoint::Presentation.slideSize
```

Gets/sets the size of the slides in the presentation.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slideSize = Vec2(400,400);
return presentation.slideSize;

### slideSizeType

```flexscript
int PowerPoint::Presentation.slideSizeType
```

Gets/sets the SlideSizeType of the presentation, an enumeration of PowerPoint.SlideSizeType.

PowerPoint.Presentation presentation = PowerPoint.Presentation.create(1);
presentation.addSlide(PowerPoint.Layout.blank);
presentation.slideSizeType = PowerPoint.SlideSizeType.OnScreen16x10;
return presentation.slideSizeType;

### template

```flexscript
string PowerPoint::Presentation.template (readonly)
```

The template the presentation is using (Read Only).

PowerPoint.Presentation presentation = PowerPoint.Presentation.open(modeldir() + "\\Presentation.pptx", 1);
return presentation.template;

