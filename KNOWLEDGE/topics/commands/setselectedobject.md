---
id: setselectedobject
name: "setselectedobject"
kind: command
module: ""
signature: "setselectedobject(obj view, obj selected)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: setselectedobject."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setselectedobject

```flexscript
setselectedobject(obj view, obj selected)
```

For developer use. Sets the selectedobject (yellow highlighted object) of the view. Note that this is different than so(). The highlighted object is accessed by the selectedobject() command.

## Example

```flexscript
setselectedobject(ownerview(c), model().first)
```

