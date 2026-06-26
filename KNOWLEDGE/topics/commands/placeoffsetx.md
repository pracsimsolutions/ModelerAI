---
id: placeoffsetx
name: "placeoffsetx"
kind: command
module: ""
signature: "placeoffsetx(obj station, obj item, obj unloadingobject) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: placeoffsetx."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# placeoffsetx

```flexscript
placeoffsetx(obj station, obj item, obj unloadingobject) -> num
```

This command queries the station for an x offset for the unloadingobject to travel from its x center in order to unload the item. For more information on offset travel, refer to the TaskExecuter documentation.

## Example

```flexscript
int xoffset = placeoffsetx(current.centerObjects[1],item,current);
```

