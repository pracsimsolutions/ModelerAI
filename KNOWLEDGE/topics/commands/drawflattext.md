---
id: drawflattext
name: "drawflattext"
kind: command
module: ""
signature: "drawflattext(str text)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: "this"
description: "FlexScript command: drawflattext."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawflattext

```flexscript
drawflattext(str text)
```

This command can only be used in a 3D view. It draws a texture-mapped text string. This uses the same functionality that draws
the names and stats below objects. If scaled correctly, it draws a very crisp, readable text. Use this in conjunction with setdrawnamefunction()
to customize the data that is shown in the box below objects. Unlike drawtext(), this command's parameter set is much more stripped down, so you
should use gl commands (i.e. fglTranslate(), fglScale(), fglColor(), etc.) to set up the location, size, and render settings before drawing
the text.

## Example

```flexscript
Add the following code to an object's Custom Draw Code trigger:

if(!param(2)) {
setdrawnamefunction(80, 15, c, param(1), 1);
} else {
fglTranslate(-40, -15, 0);
drawflattext("Hello World");
}

This will draw the text Hello World in the display box below the object
```

