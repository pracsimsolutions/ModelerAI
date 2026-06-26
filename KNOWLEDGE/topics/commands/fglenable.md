---
id: fglenable
name: "fglEnable"
kind: command
module: ""
signature: "fglEnable(num setting)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: "this"
description: "Enables an OpenGL draw setting"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglEnable

```flexscript
fglEnable(num setting)
```

Enables an OpenGL draw setting

Enables an OpenGL draw setting.
Note that for optimization purposes, FlexSim's drawing engine does not automatically restore the
enabled/disabled state of a given setting, so it is up to you, if you use this command, to restore
the setting to its default after you have executed your drawing functionality. Failing to restore
a setting to its default will result in weird rendering of other objects in the model.

Common OpenGL draw settings:

GL_LIGHTING
Turns lighting on and off.
Turn it on if polygons do not change color when they turn away from the viewer.
Default: enabled.
GL_CULL_FACE
Turns back-face culling on and off
Turn it off if you want both sides (front and back) of all polygons drawn.
Default: disabled.
GL_TEXTURE_2D
Turns texturing on and off.
Turn it off if a texture is being displayed and you don't want it there.
Default: enabled.

You can refer to standard OpenGL documentation on glEnable() for more information if needed.

## Example

```flexscript
fglEnable(GL_LIGHTING);
```

