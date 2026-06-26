---
id: stopinput
name: "stopinput"
kind: command
module: ""
signature: "stopinput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.input.stop"
description: "Deprecated, use Object.input.stop()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stopinput

> ⚠ **DEPRECATED.** Use `Object.input.stop` instead.

```flexscript
stopinput(obj object)
```

Deprecated, use Object.input.stop()

This command is deprecated. Use Object.input.stop() instead.

This command closes the input of the object just like closeinput() does, but it also keeps track of consecutive stopinput() commands, and will only open input after all stops have been resumed. Refer to closeinput() documentation for additional information.

## Example

```flexscript
stopinput(current.centerObjects[1]);
```

