---
id: fglloadidentity
name: "fglLoadIdentity"
kind: command
module: ""
signature: "fglLoadIdentity()"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: fglLoadIdentity."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglLoadIdentity

```flexscript
fglLoadIdentity()
```

Loads the identity matrix into the current OpenGL drawing context, or in other words, replaces the current matrix with the
identity matrix. A matrix is just a way to specify a whole bunch of n-dimensional transformations in one convenient package. Loading
the identity matrix is simply a way of resetting things to the origin, so there's no rotation, translation or scaling. The default
position is (0,0,0) and the default orientation is looking down the negative z-axis. You can refer to standard OpenGL documentation for
more information if needed.

## Example

```flexscript
fglLoadIdentity()
```

