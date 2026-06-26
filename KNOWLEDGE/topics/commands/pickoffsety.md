---
id: pickoffsety
name: "pickoffsety"
kind: command
module: ""
signature: "pickoffsety(obj station, obj item, obj loadingobject) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickoffsety."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickoffsety

```flexscript
pickoffsety(obj station, obj item, obj loadingobject) -> num
```

This command queries the station for a y offset for the loadingobject to travel from its y center in order to load the item. For more information on offset travel, refer to the TaskExecuter documentation.

## Example

```flexscript
int yoffset = pickoffsety(item.up,item,current);
```

