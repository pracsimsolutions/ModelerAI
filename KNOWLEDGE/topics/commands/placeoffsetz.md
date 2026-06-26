---
id: placeoffsetz
name: "placeoffsetz"
kind: command
module: ""
signature: "placeoffsetz(obj station, obj item, obj unloadingobject) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: placeoffsetz."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# placeoffsetz

```flexscript
placeoffsetz(obj station, obj item, obj unloadingobject) -> num
```

This command queries the station for a z offset for the unloadingobject to travel from its z base in order to unload the item. For more information on offset travel, refer to the TaskExecuter documentation.

## Example

```flexscript
int zoffset = placeoffsetz(current.centerObjects[1],item,current);
```

