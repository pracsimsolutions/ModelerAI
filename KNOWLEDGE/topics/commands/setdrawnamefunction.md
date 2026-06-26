---
id: setdrawnamefunction
name: "setdrawnamefunction"
kind: command
module: ""
signature: "setdrawnamefunction(num boxwidth, num boxheight, node nodefunction[, num pval1, num pval2, num pval3])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: setdrawnamefunction."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setdrawnamefunction

```flexscript
setdrawnamefunction(num boxwidth, num boxheight, node nodefunction[, num pval1, num pval2, num pval3])
```

This command gives direct access to the functionality that displays information in boxes below objects in the model.
FlexSim's 3D display mechanism works by performing multiple drawing passes over the objects in the model. First, it gathers and caches
data for what needs to be drawn. Then it goes through and draws the object shapes and calls the objects' custom draw functionality. Finally it
draws any overlays such as connectors, information boxes, etc. The setdrawnamefunction() command, if called as part of an object's custom draw
functionality, i.e. its OnDraw event or its Custom Draw Code trigger, will cause a callback to be fired when the drawing sequence gets to the
information box step for that object. To do this, call the command and specify the size that you want the box to be drawn, and pass as the nodefunction
parameter a reference to a node holding the code you want to fire to draw the box information, as well as up to 3 parameters that will be passed
as param(1), param(2), and param(3) into that nodefunction. Then implement the nodefunction code to draw any information you want, using the
drawflattext() command, or any other OpenGL drawing commands. Note that when the nodefunction is call, the OpenGL model view matrix will be set so
that the (0,0,0) point is at the top-center of the display box, and one length unit represents one pixel on the screen.

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

