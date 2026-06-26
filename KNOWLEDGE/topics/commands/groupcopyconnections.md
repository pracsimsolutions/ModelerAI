---
id: groupcopyconnections
name: "groupcopyconnections"
kind: command
module: ""
signature: "groupcopyconnections(obj object)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: groupcopyconnections."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# groupcopyconnections

```flexscript
groupcopyconnections(obj object)
```

For developer use. Copies the connections on the object passed in as param(1) onto every selected object in the model. Typically used in GUIs

## Example

```flexscript
groupcopyconnections(selectedobject(ownerview(c)));
```

