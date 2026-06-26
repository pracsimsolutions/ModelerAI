---
id: classobject
name: "classobject"
kind: command
module: ""
signature: "classobject(obj object) -> obj"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Returns the class object of an instance"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# classobject

```flexscript
classobject(obj object) -> obj
```

Returns the class object of an instance

Returns the class object of an instance.

## Example

```flexscript
if(classobject(current) == library().find("?Processor")) { pt("Processor1 is an instance of the Processor class.");pr(); }
This prints a message to the output console if current is a Processor.
```

