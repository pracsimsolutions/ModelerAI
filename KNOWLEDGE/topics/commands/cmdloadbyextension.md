---
id: cmdloadbyextension
name: "cmdloadbyextension"
kind: command
module: ""
signature: "cmdloadbyextension(str filename, num silent)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: cmdloadbyextension."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cmdloadbyextension

```flexscript
cmdloadbyextension(str filename, num silent)
```

For developer use.
Load a file by name and extension. Recognizes: Project (fsp), Session (fss), Model (fsm), View Layout (fsv). If silent is 0, the user will be asked to confirm that they are opening the correct file.

## Example

```flexscript
cmdloadbyextension("c:/mysession.fss",0)
```

