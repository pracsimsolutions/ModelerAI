---
id: relloc
name: "relloc"
kind: command
module: ""
signature: "relloc(obj object, obj containerobj, num coordinate) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Vec3.project"
description: "Deprecated, use Vec3.project()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# relloc

> ⚠ **DEPRECATED.** Use `Vec3.project` instead.

```flexscript
relloc(obj object, obj containerobj, num coordinate) -> num
```

Deprecated, use Vec3.project()

This command is deprecated. Use Vec3.project() instead.

This command returns the x, y, or z location of an object relative to an object that contains it. The containerobj does not need to be the immediate container, but does need to eventually be a container of the object. Use a coordinate value of 1,2,3 for x,y,z respectively.

## Example

```flexscript
double x = relloc(current,model(),1); double y = relloc(current,model(),2); double z = relloc(current,model(),3);
Sets x, y and z to the x,y,z positions of the current object relative to the model space.
```

