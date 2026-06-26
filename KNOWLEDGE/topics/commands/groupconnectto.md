---
id: groupconnectto
name: "groupconnectto"
kind: command
module: ""
signature: "groupconnectto(obj object, str character)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: groupconnectto."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# groupconnectto

```flexscript
groupconnectto(obj object, str character)
```

For developer use. Connects every selected object in the model to the object passed in as param(1) using the letter passed in as param(2). Typically used in GUIs.

## Example

```flexscript
groupconnectto(selectedobject(ownerview(c)),"A");
```

