---
id: resumeinput
name: "resumeinput"
kind: command
module: ""
signature: "resumeinput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.input.resume"
description: "Deprecated, use Object.input.resume()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# resumeinput

> ⚠ **DEPRECATED.** Use `Object.input.resume` instead.

```flexscript
resumeinput(obj object)
```

Deprecated, use Object.input.resume()

This command is deprecated. Use Object.input.resume() instead.

This command opens the input of an object just like openinput() does, but because it keeps track of previous stopinput() commands called on the object, it will only open input after all stops have been resumed. Refer to openinput() documentation for additional information.

## Example

```flexscript
resumeinput(current.centerObjects[1]);
```

