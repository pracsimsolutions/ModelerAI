---
id: setanimationvar
name: "setanimationvar"
kind: command
module: ""
signature: "setanimationvar(obj object, num/str animation, str varName, num/obj toValue)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: "surrogates"
description: "Sets the value of an animation variable on an object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setanimationvar

```flexscript
setanimationvar(obj object, num/str animation, str varName, num/obj toValue)
```

Sets the value of an animation variable on an object

Sets the value of an animation variable on an object. Animation variables are attributes of an animation
that may need to dynamically change for different runs of the animation, such as surrogates, time between keyframes,
or position/rotation/size values of a given keyframe. For example, you may use surrogates in building your animation.
You do this by choosing a default visual from the flowitem bin to use as you create the animation. Then when the
simulation runs, you substitute real flowitems in for those surrogates when you run the simulation. You do this
substitution by setting the value of an animation variable that you've associated with that surrogate to the actual
reference to the flowitem. Another example is you may use an animation variable to dynamically define the z position
of a keyframe at run-time.

## Example

```flexscript
setanimationvar(current, "Animation1", "Item1", current.first);
```

