---
id: powerpoint__layout
name: "PowerPoint::Layout"
kind: class
module: "PowerPoint"
signature: "PowerPoint::Layout"
aliases: []
tags: ["class", "powerpoint-module"]
deprecated: false
replacedBy: null
description: "An enumeration of integer values associated with PowerPoint slide layouts."
seeAlso: []
source: "help\\FlexScriptAPIReference\\PowerPoint\\PowerPoint.Layout.xml"
---

# PowerPoint::Layout

An enumeration of integer values associated with PowerPoint slide layouts.

The PowerPoint.Layout values are used in the PowerPoint.Presentation.addSlide method
to specify the layout the new slide will be created with.

## Properties

### blank

```flexscript
int PowerPoint::Layout.blank (readonly)
```

Blank slide.

### custom

```flexscript
int PowerPoint::Layout.custom (readonly)
```

Custom layout, defined by the template.

### text

```flexscript
int PowerPoint::Layout.text (readonly)
```

Has a title textbox with a large textbox below it.

### title

```flexscript
int PowerPoint::Layout.title (readonly)
```

Has a title textbox and a subtitle textbox.

