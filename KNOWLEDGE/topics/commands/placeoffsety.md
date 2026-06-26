---
id: placeoffsety
name: "placeoffsety"
kind: command
module: ""
signature: "placeoffsety(obj station, obj item, obj unloadingobject) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: placeoffsety."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# placeoffsety

```flexscript
placeoffsety(obj station, obj item, obj unloadingobject) -> num
```

This command queries the station for a y offset for the unloadingobject to travel from its y center in order to unload the item. For more information on offset travel, refer to the TaskExecuter documentation.

## Example

```flexscript
int yoffset = placeoffsety(current.centerObjects[1],item,current);
```

