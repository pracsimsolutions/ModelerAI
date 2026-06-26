---
id: cleannodeflexscript
name: "cleannodeflexscript"
kind: command
module: ""
signature: "cleannodeflexscript(node startnode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: cleannodeflexscript."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cleannodeflexscript

```flexscript
cleannodeflexscript(node startnode)
```

For developer use. This command recursively deletes Flexscript trees starting at the node specified.

## Example

```flexscript
cleannodeflexscript(model());
This cleans all the Flexscript nodes in the model.
```

