---
id: pickoffsetz
name: "pickoffsetz"
kind: command
module: ""
signature: "pickoffsetz(obj station, obj item, obj loadingobject) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickoffsetz."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickoffsetz

```flexscript
pickoffsetz(obj station, obj item, obj loadingobject) -> num
```

This command queries the station for a z offset for the loadingobject to travel from its z base in order to load the item. For more information on offset travel, refer to the TaskExecuter documentation.

## Example

```flexscript
int zoffset = pickoffsetz(item.up,item,current);
```

