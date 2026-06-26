---
id: fscheckall
name: "fscheckall"
kind: command
module: ""
signature: "fscheckall(node startingnode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: fscheckall."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fscheckall

```flexscript
fscheckall(node startingnode)
```

For developer use. This command checks for errors in all flexscript nodes under and including the specified startingnode and outputs the results to the compiler console if it finds atleast one error.

## Example

```flexscript
fscheckall(main());
```

