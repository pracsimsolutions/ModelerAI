---
id: setstate_s
name: "setstate_s"
kind: command
module: ""
signature: "setstate_s(obj object, str statename)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.stats.state"
description: "FlexScript command: setstate_s."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setstate_s

> ⚠ **DEPRECATED.** Use `Object.stats.state` instead.

```flexscript
setstate_s(obj object, str statename)
```

This command is deprecated. Use Object.stats.state().valueString = stateStr;

For development use. Set the state of an object using the actual name of one of the states. The state names must be as they appear in a state pie chart.

## Example

```flexscript
setstate_s(current,"busy")
```

