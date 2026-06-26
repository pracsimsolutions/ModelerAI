---
id: resumeoutput
name: "resumeoutput"
kind: command
module: ""
signature: "resumeoutput(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.output.resume"
description: "Deprecated, use Object.output.resume()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# resumeoutput

> ⚠ **DEPRECATED.** Use `Object.output.resume` instead.

```flexscript
resumeoutput(obj object)
```

Deprecated, use Object.output.resume()

This command is deprecated. Use Object.output.resume() instead.

This command opens the output of an object just like openoutput() does, but because it keeps track of previous stopoutput() commands called on the object, it will only open output after all stops have been resumed. Refer to openoutput() documentation for additional information.

## Example

```flexscript
resumeoutput(current.centerObjects[1]);
```

