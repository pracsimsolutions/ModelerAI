---
id: tracemaskobject
name: "tracemaskobject"
kind: command
module: ""
signature: "tracemaskobject(obj object)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: tracemaskobject."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# tracemaskobject

```flexscript
tracemaskobject(obj object)
```

For developer use. Set an object to be included in the event reporting in the trace debugger. See View|Trace Debugger...

## Example

```flexscript
tracemaskobject(Model.find("Processor3"))
```

