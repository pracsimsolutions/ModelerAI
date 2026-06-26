---
id: stopoutput
name: "stopoutput"
kind: command
module: ""
signature: "stopoutput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.output.stop"
description: "Deprecated, use Object.output.stop()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stopoutput

> ⚠ **DEPRECATED.** Use `Object.output.stop` instead.

```flexscript
stopoutput(obj object)
```

Deprecated, use Object.output.stop()

This command is deprecated. Use Object.output.stop() instead.

This command closes the output of the object just like closeoutput() does, but it also keeps track of consecutive stopoutput() commands, and will only open output after all stops have been resumed. Refer to closeoutput() documentation for additional information.

## Example

```flexscript
stopoutput(current.centerObjects[1]);
```

