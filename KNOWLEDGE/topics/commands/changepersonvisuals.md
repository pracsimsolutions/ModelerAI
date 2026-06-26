---
id: changepersonvisuals
name: "changepersonvisuals"
kind: command
module: ""
signature: "changepersonvisuals(treenode person, string category, num/string index [, Array color])"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Change the person's visuals (Head, Hair, Skin Texture, Clothes)"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# changepersonvisuals

```flexscript
changepersonvisuals(treenode person, string category, num/string index [, Array color])
```

Change the person's visuals (Head, Hair, Skin Texture, Clothes)

Change the person's visuals (Head, Hair, Skin Texture, Clothes). This can be done dynamically during the model run.

## Example

```flexscript
changepersonvisuals(person, "Shirt", "Scrubs", Color.red)
```

