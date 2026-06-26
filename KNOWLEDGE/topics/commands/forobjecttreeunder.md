---
id: forobjecttreeunder
name: "forobjecttreeunder"
kind: command
module: ""
signature: "forobjecttreeunder(node container) { }"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: forobjecttreeunder."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# forobjecttreeunder

```flexscript
forobjecttreeunder(node container) { }
```

Execute a block of code for each node that is in the tree under the node specified as container, including all sub-trees. To access the node that is currently active in the loop, the "a" command is used. The forobjecttreeunder() command can only be used in flexscript, not C++.

## Example

```flexscript
forobjecttreeunder(model()) { pt(a.name); pr(); }
This prints to the outputconsole the name of every object in the model, including those inside other objects.
```

