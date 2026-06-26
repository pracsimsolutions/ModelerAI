---
id: pickoffsetx
name: "pickoffsetx"
kind: command
module: ""
signature: "pickoffsetx(obj station, obj item, obj loadingobject) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickoffsetx."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickoffsetx

```flexscript
pickoffsetx(obj station, obj item, obj loadingobject) -> num
```

This command queries the station for an x offset for the loadingobject to travel from its x center in order to load the item. For more information on offset travel, refer to the TaskExecuter documentation.

## Example

```flexscript
int xoffset = pickoffsetx(item.up,item,current);
```

