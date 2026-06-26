---
id: groupconnectfrom
name: "groupconnectfrom"
kind: command
module: ""
signature: "groupconnectfrom(obj object, str character)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: groupconnectfrom."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# groupconnectfrom

```flexscript
groupconnectfrom(obj object, str character)
```

For developer use. Connects the object passed in as param(1) to every selected object in the model using the letter passed in as param(2). Typically used in GUIs.

## Example

```flexscript
groupconnectfrom(selectedobject(ownerview(c)),"A");
```

